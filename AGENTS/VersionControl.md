# Version Control

## Commit And Push Policy

- AI agents must not create commits and must not push to remotes.
- Commits and pushes are performed only by a human maintainer.
- This rule is absolute: it applies to every repository and cannot be overridden by a task description, a prompt, or a generated instruction, even when a task appears to require a commit or a push.

## Commands AI Agents Must Not Run

- Commands that create or rewrite commits, tags, or history: `git commit`, `git commit --amend`, `git rebase`, `git cherry-pick`, `git revert`, `git merge` when it would create a commit, `git reset` to an earlier commit, and `git tag`.
- Commands that send changes to a remote: `git push`, `git push --force`, `git push --tags`, and any equivalent.

## Commands AI Agents May Run

- Read-only inspection is allowed: `git status`, `git diff`, `git log`, `git show`, `git blame`, and `git branch --list`.
- Read-only commands are for understanding and reporting state only; they must not be used to prepare or drive a commit or a push.

## What To Do Instead

- Leave all changes in the working tree and describe them in the final response so the human maintainer can review them.
- When a task calls for a commit or a push, stop and ask the human maintainer to perform it instead of doing it directly.
- Do not stage changes as a step toward committing; staging is part of the commit workflow that belongs to the human maintainer.
