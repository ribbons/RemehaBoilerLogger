#!/usr/bin/perl -w

# Copyright © 2017 Matt Robinson

use strict;
use warnings FATAL => 'all';

#use Const::Fast;
use XML::LibXML;

sub uint_from_bits
{
    ## no critic (ProhibitMagicNumbers)
    my $bits = shift or die 'Missing bits value';
    return 'uint8_t'  if ($bits <=  8);
    return 'uint16_t' if ($bits <= 16);
    return 'uint32_t' if ($bits <= 32);
    return 'uint64_t' if ($bits <= 64);
    die 'Value too large';
}

use constant BITS_IN_BYTE => 8;

use constant EXPRESSIONS => {
    'A' => ['uint8_t', 8],
};

use constant SPECIAL => {
    5 => ['uint64_t', 40],
    6 => ['uint8_t',   8],
};

my $configfile = shift or die 'First parameter should be configuration file';
my $langfile = shift or die 'Second parameter should be language file';

my $langxml = XML::LibXML->load_xml(location => $langfile);
my @textnodes = $langxml->findnodes('/language/text');
my %langtext;

foreach my $textnode (@textnodes)
{
    $langtext{$textnode->getAttribute('id')} = $textnode->getAttribute('value');
}

my $configxml = XML::LibXML->load_xml(location => $configfile);
my $configurationsnode = ($configxml->findnodes('/configuration/configurations[@name="identification"]'))[0];

my @confignodes = $configurationsnode->findnodes('./config');
my @fields;

foreach my $confignode (@confignodes)
{
    my $fieldid = $confignode->findvalue('./present/@name.nr');

    if($langtext{$fieldid} =~ m/d[FU]-code/)
    {
        # These aren't actually returned in the identify response, Recom
        # appears to read block 0x10 from EEPROM and append it to the response
        next;
    }

    my $function = ($confignode->findnodes('./function'))[0];

    my ($type, $size);
    my $conv = 'std::to_string(%id%)';

    if($function->hasAttribute('number'))
    {
        my $number = $function->getAttribute('number');
        ($type, $size) = @{SPECIAL->{$number}};
        $conv = "SpecialDataConv$number(%id%)";
    }
    else
    {
        my $expr = $function->getAttribute('expression');
        ($type, $size) = @{EXPRESSIONS->{$expr}};
    }

    push @fields, {
        ident => "f$fieldid",
        start => $function->getAttribute('byte') * BITS_IN_BYTE,
        bits  => $size,
        name  => $langtext{$fieldid},
        type  => $type,
        conv  => $conv =~ s/%id%/unpack.f$fieldid/gr,
    };
}

my $lastbit = 0;

print "struct identification\n{\n";

foreach my $field (sort {$a->{start} <=> $b->{start}} @fields)
{
    if($field->{start} > $lastbit)
    {
        my $length = $field->{start} - $lastbit;
        print '    '.uint_from_bits($length)." : $length;\n";
    }

    print "    $field->{type} $field->{ident} : $field->{bits};\n";
    $lastbit = $field->{start} + $field->{bits};
}

print "} __attribute__((__packed__));\n\n";

foreach my $field (sort {$a->{start} <=> $b->{start}} @fields)
{
    print "    this->values[\"$field->{name}\"] = $field->{conv};\n";
}
