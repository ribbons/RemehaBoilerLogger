# frozen_string_literal: true

# Copyright © 2022 Matt Robinson

module Overcommit
  module Hook
    module PreCommit
      class Cppcheck < Base
        MESSAGE_REGEX = /^(?<file>.+):(?<line>\d+):/.freeze

        def run
          result = execute(command, args: applicable_files)

          extract_messages(
            result.stderr.chomp.split("\n"),
            MESSAGE_REGEX
          )
        end
      end
    end
  end
end
