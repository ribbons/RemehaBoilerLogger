# frozen_string_literal: true

# Copyright © 2021 Matt Robinson
#
# SPDX-License-Identifier: GPL-3.0-or-later

module Overcommit
  module Hook
    module PreCommit
      class PerlCritic < Base
        MESSAGE_REGEX = /^(?<file>.+):(?<line>\d+):\d:/

        def run
          result = execute(command, args: applicable_files)
          return :pass if result.success?

          # example message:
          #   path/to/file.pl:89:1:Useless '## no critic' annotation
          extract_messages(
            result.stdout.chomp.split("\n"),
            MESSAGE_REGEX
          )
        end
      end
    end
  end
end
