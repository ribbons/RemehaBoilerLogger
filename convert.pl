#!/usr/bin/perl -w

# Copyright © 2017-2024 Matt Robinson
# Copyright © 2021 Mattias Jonsson
#
# SPDX-License-Identifier: GPL-3.0-or-later

use strict;
use warnings FATAL => 'all';

use XML::LibXML;

sub uint_from_bits
{
    my $bits = shift or die 'Missing bits value';
    return 'uint8_t'  if ($bits <=  8);
    return 'uint16_t' if ($bits <= 16);
    return 'uint32_t' if ($bits <= 32);
    return 'uint64_t' if ($bits <= 64);
    die 'Value too large';
}

use constant BITS_IN_BYTE => 8;

use constant EXPRESSIONS => {
    'A'                     => ['uint8_t',   8, '$id',           1],
    'A x 0.1'               => ['uint8_t',   8, '$id',         0.1],
    'A x 100'               => ['uint8_t',   8, '$id',         100],
    'A.0 + B.1'             => ['uint16_t', 16, '$id',           1],
    '(A.1 + B.0) x 8'       => ['uint16_t', 16, 'ntohs($id)',    8],
    '(A.1 + B.0) x 10'      => ['uint16_t', 16, 'ntohs($id)',   10],
    'sgn(A.0 + B.1) x 0.01' => ['int16_t',  16, '$id',        0.01],
};

use constant SPECIAL => {
    5 => ['uint64_t', 40],
    6 => ['uint8_t',   8],
};

binmode STDOUT, ':encoding(UTF-8)' or die 'Failed to set encoding';

my $configfile = shift or die "First parameter should be configuration file\n";
my $langfile = shift or die "Second parameter should be language file\n";
my $configuration = shift or die "Third parameter should be configuration name\n";

my $langxml = XML::LibXML->load_xml(location => $langfile);
my @textnodes = $langxml->findnodes('/language/text');
my %langtext;

foreach my $textnode (@textnodes)
{
    $langtext{$textnode->getAttribute('id')} = $textnode->getAttribute('value') =~ s/ +$//r;
}

my $configxml = XML::LibXML->load_xml(location => $configfile);
my ($configurationsnode) = $configxml->findnodes("/configuration/configurations[\@name='$configuration']");

unless(defined $configurationsnode)
{
    die "Configuration \"$configuration\" was not found in $configfile\n"
}

my @confignodes = $configurationsnode->findnodes('./config');
my @fields;
my $fieldnum = 1;

foreach my $confignode (@confignodes)
{
    my ($present) = $confignode->findnodes('./present');
    my $name = $langtext{$present->getAttribute('name.nr')};

    if($present->hasAttribute('unit.nr'))
    {
        my $unit = $langtext{$present->getAttribute('unit.nr')};
        $name .= " [$unit]";
    }

    my ($function) = $confignode->findnodes('./function');

    my ($type, $size, $func);
    my $start = $function->getAttribute('byte') * BITS_IN_BYTE;
    my @args = ("unpack.f$fieldnum");

    if($function->hasAttribute('number'))
    {
        my $number = $function->getAttribute('number');

        if(!defined SPECIAL->{$number})
        {
            die "Unimplemented special data conversion $number encountered";
        }

        ($type, $size) = @{SPECIAL->{$number}};
        $func = "SpecialDataConv$number";
    }
    else
    {
        if($function->hasAttribute('bit'))
        {
            $type = 'bool';
            $size = 1;
            $start += $function->getAttribute('bit');

            if($function->getAttribute('invert') eq 'true')
            {
                $args[0] = "!$args[0]";
            }
        }
        else
        {
            my $expr = $function->getAttribute('expression');
            die "Unknown expression \"$expr\"" unless exists EXPRESSIONS->{$expr};
            my ($cexpr, $scale);
            ($type, $size, $cexpr, $scale) = @{EXPRESSIONS->{$expr}};

            $args[0] = $cexpr =~ s/\$id/$args[0]/gr;

            $func = 'FormatNumber';
            push @args, $scale;

            if($confignode->findnodes('./limits'))
            {
                my ($limits) = $confignode->findnodes('./limits');
                my $min = $limits->getAttribute('min');
                my $max = $limits->getAttribute('max');
                my ($mintext, $maxtext) = ($min, $max);

                if($limits->hasAttribute('min.nr'))
                {
                    $mintext = $langtext{$limits->getAttribute('min.nr')};
                }

                if($limits->hasAttribute('max.nr'))
                {
                    $maxtext = $langtext{$limits->getAttribute('max.nr')};
                }

                push @args, $min, $max, "\"$mintext\"", "\"$maxtext\"";
            }
        }

        if($confignode->findnodes('./selections|./node-ref[@type="selections"]'))
        {
            my $selid = $confignode->findvalue('.//@id');
            $func = $selid =~ s/(?:^|[.])([a-z])/\U$1/gr.'String';
            @args = ($args[0]);
        }
    }

    push @fields, {
        ident => "f$fieldnum",
        start => $start,
        bits  => $size,
        name  => $name,
        type  => $type,
        func  => $func,
        args  => \@args,
    };

    $fieldnum++;
}

my $lastbit = 0;

print "struct $configuration\n{\n";

foreach my $field (sort {$a->{start} <=> $b->{start}} @fields)
{
    if($field->{start} > $lastbit)
    {
        my $length = $field->{start} - $lastbit;
        my $extra = '';

        while($length > 64)
        {
            $extra .= "    uint64_t : 64;\n";
            $length -= 64;
        }

        print "$extra    ".uint_from_bits($length)." : $length;\n";
    }

    print "    $field->{type} $field->{ident} : $field->{bits};\n";
    $lastbit = $field->{start} + $field->{bits};
}

print "} __attribute__((__packed__));\n\n# -------\n\n";

foreach my $field (sort {$a->{start} <=> $b->{start}} @fields)
{
    print "    this->values[\"$field->{name}\"] = $field->{func}(".join(', ', @{$field->{args}}).");\n";
}
