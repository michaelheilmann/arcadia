# Version Control

## Working Branches And Commits

- AI agents may create dedicated working branches from `develop` for user-selected tasks. Never branch from another task branch.
- AI working branch names must begin with the primary Arcadia ALM issue number, followed by a concise lowercase, hyphen-separated task description: `<issue-number>-<description>` (for example, `185-standardize-ai-branch-commit-names`).
- Each AI working branch must have exactly one primary Arcadia ALM issue. Arcadia ALM (`michaelheilmann/arcadia-alm`) is the canonical issue-number namespace across Arcadia repositories; the branch number and its task commit's issue number must match.
- The task commit subject must begin with `[#<issue-number>] ` followed by a concise imperative description (for example, `[#185] Standardize AI branch and commit naming`). Intermediate task commits should also use this prefix.
- Apply these conventions to newly created AI working branches. Do not automatically rename existing branches solely to satisfy these conventions.
- AI agents may edit files, stage changes, create commits, amend commits, rebase their own working branches, and push those working branches to the remote.
- AI agents may create multiple intermediate commits on their dedicated working branches during development.
- Before declaring a task complete or handing it over, squash all task commits into exactly one commit not present in `develop`.
- Before handover, rebase the working branch onto the latest `develop`, then verify that exactly one task commit remains ahead of `develop`.
- Push the working branch so that the result is recoverable from GitHub, and document its branch, commit, and verification results in the corresponding Arcadia ALM issue.

## Pull Requests And Merges: Maintainer Only

- AI agents must not create or submit pull requests (also called merge requests), including drafts.
- AI agents must not merge pull requests, merge branches, or otherwise integrate a working branch into `develop`, `main`, or another branch.
- The human maintainer alone decides whether to open a pull/merge request and whether, when, and how to merge.

## Permitted Git Operations

- Read-only inspection includes `git status`, `git diff`, `git log`, `git show`, `git blame`, and `git branch --list`.
- On the dedicated task branch, AI agents may use `git switch -c`, `git add`, `git commit`, `git commit --amend`, `git rebase`, and `git push` to follow the rules above.
- After rebasing their own task branch, AI agents may use `git push --force-with-lease` for that branch only, after checking the remote state. Never use an unconditional force push.
- Equivalent GitHub operations are allowed only when they observe the same branch, commit, and merge restrictions.

## Prohibited Operations And Safeguards

- Never use `git merge`, create a pull/merge request, or invoke a merge action on behalf of the maintainer.
- Never commit directly to, rebase, reset, or force-push `develop`, `main`, or someone else's working branch.
- Never use `git push --force`, `git push --tags`, or history-destructive commands outside the active task branch.
- Never discard unrelated working-tree changes or overwrite remote changes without first resolving the conflict safely.
- Verify working-tree status, branch ancestry, final diff, tests, and the one-commit invariant before handover. Record limitations and outstanding steps in the issue.
