# frozen_string_literal: true

# Copyright © 2022-2025 Matt Robinson

module Overcommit
  module Hook
    module PreCommit
      class Cppcheck < Base
        MESSAGE_REGEX = /^(?<file>.+):(?<line>\d+):/

        def run
          runcmd = command

          if @context.class.name != 'Overcommit::HookContext::RunAll'
            runcmd += '--suppress=unusedFunction'
          end

          result = execute(runcmd, args: applicable_files)

          extract_messages(
            result.stderr.chomp.split("\n"),
            MESSAGE_REGEX
          )
        end
      end
    end
  end
end
