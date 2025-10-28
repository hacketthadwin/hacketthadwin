Git Problems Log
=================

This file documents the git-related problems encountered during recovery and the exact commands used to inspect/repair the repository. Each entry lists the command, purpose, problem observed, and how it was resolved (or next steps).

1) Backup before recovery
-------------------------
Command:
  robocopy my-app my-app-backup /MIR
Use / purpose:
  Make a complete filesystem mirror backup of the repository before running recovery commands.
Problem faced:
  N/A (precaution)
Resolution:
  Backup created: my-app-backup (prevents further data loss).

2) Check repository status and branches
--------------------------------------
Commands:
  git status --porcelain --untracked-files=all
  git branch --show-current
  git branch -a
  git remote -v
Use / purpose:
  Inspect working tree, current branch, all local/remote branches and remote URL.
Problem faced:
  Files were missing/changed unexpectedly after an attempted push; needed to know current state.
Resolution:
  Outputs showed modified/deleted and untracked files which guided further inspection.

3) Find a recovery commit (reflog)
---------------------------------
Command:
  git reflog -n 200
Use / purpose:
  Search the reflog for recent operations and commits (including lost commits from rebases/resets).
Problem faced:
  Need to locate the last commit that contained the lost work.
Outcome:
  Found commit `d47a246` with message "Safe backup after rebase issue resolved" which contained the recovered files.

4) Inspect commits and commit contents
--------------------------------------
Commands:
  git log --oneline --graph --decorate -n 50
  git show --name-only <commit-hash>
  type .git\logs\HEAD  (inspect raw logs)
Use / purpose:
  Confirm the commit(s) and list files changed in the candidate recovered commit.
Problem faced:
  Needed to make sure the identified commit indeed contained the files to recover.
Outcome:
  Verified `d47a246` contained the expected files (many added/modified files).

5) Create a safety branch for the recovery commit
-------------------------------------------------
Command:
  git checkout -b recovered-work
Use / purpose:
  Create a named branch pointing at the current HEAD (the recovered commit) so we can safely inspect and restore.
Problem faced:
  Avoid accidental overwrites on main; preserve recovered commit.
Resolution:
  `recovered-work` branch created and checked out.

6) Restore working tree to the recovered commit
----------------------------------------------
Command:
  git reset --hard HEAD
Use / purpose:
  Reset working tree to the state of the current branch HEAD (recovered commit) to restore files.
Problem faced:
  Risky if run without a backup, but safe because a filesystem backup and branch were created.
Resolution:
  Working tree restored to the recovered commit (files returned).

7) Push recovery branch to remote (preserve on GitHub)
-----------------------------------------------------
Command:
  git push origin recovered-work
Use / purpose:
  Store the recovered commit on GitHub so it's not lost and can be reviewed or merged.
Problem faced:
  None; branch pushed successfully.

8) Inspect for lost/unreachable objects
--------------------------------------
Commands:
  git fsck --lost-found
  dir .git\lost-found  (list recovered blobs)
Use / purpose:
  Try to find unreachable blobs or commits if some content was not part of commits.
Problem faced:
  No helpful unreachable blobs were produced (not required because we had the reflog commit).
Outcome:
  Proceeded with recovered commit found in reflog.

9) Try to merge recovered branch into main (initial attempt)
----------------------------------------------------------
Commands:
  git fetch origin
  git checkout main
  git pull origin main
  git merge --no-ff recovered-work -m "Merge recovered-work: restore lost work (d47a246)"
Use / purpose:
  Merge recovered changes into main locally.
Problem faced:
  Git reported "fatal: refusing to merge unrelated histories" because branches had different roots.
Resolution:
  Decided not to force-unrelated merge; used a safer branch-based approach instead (see next steps).

10) Create a branch from current main and apply recovered files on top
--------------------------------------------------------------------
Commands:
  git checkout -b merge-main-with-recovery origin/main
  git checkout recovered-work -- .
  git add -A
  git commit -m "Restore recovered-work content on top of main (d47a246)"
  git push -u origin merge-main-with-recovery
Use / purpose:
  This copies the recovered working tree into a new branch based off the current `main` (no history merge). This creates a normal commit on top of main with the recovered files.
Problem faced:
  Needed a way to bring recovered content into main without rewriting history or forcing unrelated merges.
Outcome:
  Branch `merge-main-with-recovery` created and pushed; a PR can be opened/merged safely on GitHub.

11) Inspect differences between origin/main and recovered-work
-----------------------------------------------------------
Command:
  git diff --name-status origin/main..recovered-work
Use / purpose:
  See which files differ. This explained why the PR only updated the drizzle files.
Problem faced:
  Only the drizzle migration files appeared as differences (D), meaning most recovered content was already equivalent to main.
Outcome:
  Realized that only drizzle changes needed merging; other recovered files were already present or identical on main.

12) Alternative recovery commands used or suggested
--------------------------------------------------
- Check for stashes:
  git stash list
  git stash show -p stash@{0}
  git stash apply stash@{0}
Use / purpose:
  In case work had been stashed prior to the incident.

- Inspect specific commits / blobs:
  git show <blob-or-commit-hash>
  git fsck --full --no-reflogs --unreachable
Use / purpose:
  Low-level recovery when commits are not reachable.

13) Final merge options (what we recommended)
--------------------------------------------
Option A (Recommended): Create PR from `merge-main-with-recovery` → `main` and merge via GitHub UI.
  Commands to sync locally after merge:
    git checkout main
    git pull origin main

Option B (CLI merge): Merge branch into main locally and push
  git checkout main
  git pull origin main
  git merge --no-ff merge-main-with-recovery -m "Merge recovered work"
  git push origin main

Option C (Rebase recovered onto main) — advanced
  git checkout recovered-work
  git rebase origin/main
  # resolve rebase conflicts if any; then:
  git checkout main
  git merge recovered-work
  git push origin main

14) Safety reminders used during recovery
----------------------------------------
- Always create a filesystem backup before destructive commands (robocopy used).
- Create a safety branch (recovered-work) pointing at the found commit before any resets.
- Prefer safe PR merges to forceful history rewrites. If force is needed, use --force-with-lease to reduce accidental clobbering.

If you want this converted into a small shell script to reproduce the non-destructive recovery steps, say so and I'll generate it.

End of log.
