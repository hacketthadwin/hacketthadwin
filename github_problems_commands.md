
# Git Problems Log

This document records each git-related problem encountered during the recovery process. Every numbered entry below follows the same structure so it's easy to scan:

- Command(s): the exact commands used (fenced code block)
- Use / Purpose: why the commands were run
- Problem faced: what went wrong or what we were looking for
- Resolution / Outcome: how it was resolved or what we learned

---

## 1) Backup before recovery

**Command(s)**
```powershell
robocopy my-app my-app-backup /MIR
```

**Use / Purpose**

Create a full filesystem mirror of the repository to ensure we can recover if further operations go wrong.

**Problem faced**

Precautionary step — no problem encountered.

**Resolution / Outcome**

Backup created at `my-app-backup`.

---

## 2) Check repository status and branches

**Command(s)**
```bash
git status --porcelain --untracked-files=all
git branch --show-current
git branch -a
git remote -v
```

**Use / Purpose**

Inspect the working tree, current branch, and local/remote branches to understand repository state.

**Problem faced**

Files were missing/changed after an attempted push; we needed to see what was different locally vs remote.

**Resolution / Outcome**

The output identified modified/deleted and untracked files and guided the next steps (reflog, fsck).

---

## 3) Find a recovery commit (reflog)

**Command(s)**
```bash
git reflog -n 200
```

**Use / Purpose**

Search recent operations (including commits that may no longer be reachable from any branch) to find the last point where the lost work existed.

**Problem faced**

Needed to locate the most recent commit containing the lost work.

**Resolution / Outcome**

Found commit `d47a246` with message "Safe backup after rebase issue resolved" which included the recovered files.

---

## 4) Inspect commits and commit contents

**Command(s)**
```bash
git log --oneline --graph --decorate -n 50
git show --name-only <commit-hash>
type .git\\logs\\HEAD   # Windows: inspect raw reflog entries
```

**Use / Purpose**

Confirm the recovered commit and list which files it changed or added.

**Problem faced**

Need to verify that the identified commit indeed contained all the expected files.

**Resolution / Outcome**

Verified that `d47a246` contained many added/modified files matching the lost work.

---

## 5) Create a safety branch for the recovery commit

**Command(s)**
```bash
git checkout -b recovered-work
```

**Use / Purpose**

Create a named branch pointing at the current HEAD (which is the recovered commit) so we can work safely without altering other refs.

**Problem faced**

Avoid overwriting `main` or losing the recovered commit during further operations.

**Resolution / Outcome**

`recovered-work` branch created and checked out.

---

## 6) Restore working tree to the recovered commit

**Command(s)**
```bash
git reset --hard HEAD
```

**Use / Purpose**

Reset the working tree to the state of the current branch HEAD to bring the recovered files back into the filesystem.

**Problem faced**

Risky if done without a backup (we had a filesystem backup).

**Resolution / Outcome**

Working tree restored to the recovered commit; files returned.

---

## 7) Push recovery branch to remote (preserve on GitHub)

**Command(s)**
```bash
git push origin recovered-work
```

**Use / Purpose**

Store the recovered commit on GitHub to prevent further local-only loss.

**Problem faced**

None — the push succeeded.

**Resolution / Outcome**

Remote branch `recovered-work` contains the recovered commit.

---

## 8) Inspect for lost/unreachable objects

**Command(s)**
```bash
git fsck --lost-found
dir .git\\lost-found   # Windows
```

**Use / Purpose**

Try to locate unreachable blobs/objects if some work had not been committed.

**Problem faced**

No useful unreachable blobs were produced — we already had the reflog commit, which was sufficient.

**Resolution / Outcome**

Moved forward with the `d47a246` reflog commit.

---

## 9) Try to merge recovered branch into main (initial attempt)

**Command(s)**
```bash
git fetch origin
git checkout main
git pull origin main
git merge --no-ff recovered-work -m "Merge recovered-work: restore lost work (d47a246)"
```

**Use / Purpose**

Attempt a normal merge of recovered content into `main`.

**Problem faced**

Git reported: "fatal: refusing to merge unrelated histories" — branches appeared to have distinct roots (often happens after force-pushes or an unrelated import).

**Resolution / Outcome**

Stopped and chose a safer approach: create a new branch based on `origin/main` and copy recovered files into it (next step).

---

## 10) Create a branch from current main and apply recovered files on top

**Command(s)**
```bash
git checkout -b merge-main-with-recovery origin/main
git checkout recovered-work -- .
git add -A
git commit -m "Restore recovered-work content on top of main (d47a246)"
git push -u origin merge-main-with-recovery
```

**Use / Purpose**

Create a clean commit on top of `main` that contains the recovered working tree (no history rewriting).

**Problem faced**

Needed a way to bring recovered content into `main` without rewriting history or forcing unrelated merges.

**Resolution / Outcome**

Branch `merge-main-with-recovery` created and pushed. A PR was opened so the changes could be merged safely via GitHub UI.

---

## 11) Inspect differences between origin/main and recovered-work

**Command(s)**
```bash
git diff --name-status origin/main..recovered-work
```

**Use / Purpose**

Find which files actually differed so the PR contents could be validated.

**Problem faced**

Only drizzle migration SQL files showed up as differences (deleted), meaning most recovered files were already present or identical on `main`.

**Resolution / Outcome**

Confirmed only the migration files required attention; other recovered content was already on `main`.

---

## 12) Alternative recovery commands used or suggested

**Commands (stashes)**
```bash
git stash list
git stash show -p stash@{0}
git stash apply stash@{0}
```

**Use / Purpose**

Check for any stashed changes that may contain the lost work.

**Commands (low-level inspect)**
```bash
git show <blob-or-commit-hash>
git fsck --full --no-reflogs --unreachable
```

**Use / Purpose**

Inspect unreachable objects or specific blobs when commits are not reachable via branch refs.

**Problem faced / Outcome**

Useful when commits are not in refs; in this case the reflog commit was sufficient and low-level recovery wasn't needed.

---

## 13) Final merge options (recommended approaches)

**Option A (Recommended)** — PR merge via GitHub UI

Create a PR from `merge-main-with-recovery` → `main`, review, and merge. Then sync locally:

```bash
git checkout main
git pull origin main
```

**Option B (CLI merge)**

```bash
git checkout main
git pull origin main
git merge --no-ff merge-main-with-recovery -m "Merge recovered work"
git push origin main
```

**Option C (Rebase recovered onto main) — advanced**

```bash
git checkout recovered-work
git rebase origin/main
# resolve rebase conflicts if any
git checkout main
git merge recovered-work
git push origin main
```

Choose A for the safest, reviewable flow.

---

## 14) Safety reminders used during recovery

- Always create a filesystem backup before destructive commands (`robocopy`).
- Create a safety branch (`recovered-work`) pointing at the found commit before any resets.
- Prefer safe PR merges to forceful history rewrites. If force is needed, use `--force-with-lease` instead of a plain `--force`.

If you'd like, I can convert the non-destructive sequence (steps 1 → 10) into a PowerShell script you can run locally. Want that?

---

---

## Appendix — Push local changes to GitHub (start → finish)

This is a practical, step-by-step workflow for pushing local work to GitHub, plus common problems you may hit and how to resolve them.

### Typical workflow (safe, recommended)

1) Check current state

```bash
git status
git branch --show-current
```

Why: see what branch you're on and whether there are uncommitted changes.

Common problems:
- "nothing to commit" — no changes staged
- Untracked files you meant to include

2) Stage changes

```bash
git add <file1> <file2>
# or all changes:
git add -A
```

3) Commit

```bash
git commit -m "A clear, concise message describing the change"
```

Problems:
- pre-commit hooks fail — run the hook commands locally (lint/tests), fix issues
- large files rejected by Git — consider Git LFS

4) Sync remote (recommended: rebase workflow to minimize merge commits)

```bash
git fetch origin
# If working on feature branch (recommended):
git rebase origin/main
# or, if you prefer merge:
git merge origin/main
```

Why: bring your branch up-to-date with remote before pushing to avoid non-fast-forward errors.

Problems and fixes:
- Merge conflicts during rebase/merge
	- Resolve conflicts in files, then:
		```bash
		git add <resolved-files>
		git rebase --continue   # if rebasing
		# or
		git commit              # if merging and a commit is required
		```
- "refusing to merge unrelated histories"
	- Occurs when histories don't share a common root (e.g., imported repo). Use this only if you intentionally want to combine histories:
		```bash
		git merge origin/main --allow-unrelated-histories
		```
	- Prefer creating a new branch and copying files instead of forcing unrelated merges.

5) Push your branch

```bash
git push -u origin HEAD
```

If pushing to `main` directly (only when appropriate):

```bash
git push origin main
```

Common push problems:
- "non-fast-forward" / rejected
	- Usually because remote has new commits. Fix by fetching and rebasing/merging as in step 4.
- Authentication issues (403/401)
	- Check your Git remote URL (HTTPS vs SSH) and credential manager. For HTTPS, make sure a valid PAT (personal access token) is configured when required.
- Large file errors
	- Use Git LFS or remove the file from history.
- Branch protected by policy
	- Create a PR; a direct push may be blocked by branch protection rules.

6) Open a Pull Request (recommended)

Create a PR from your branch to `main` on GitHub. Add reviewers and CI will run.

7) After merge — sync your local main

```bash
git checkout main
git pull origin main
```

### Quick troubleshooting commands

- Show differences between your branch and remote main:

```bash
git fetch origin
git log --oneline --graph --decorate origin/main..HEAD
git diff --name-status origin/main..HEAD
```

- Abort a failing rebase:

```bash
git rebase --abort
```

- Restore working tree from last commit (dangerous if you have uncommitted work):

```bash
git reset --hard HEAD
```

- Undo a bad local commit while keeping changes staged:

```bash
git reset --soft HEAD~1
```

### Common pitfalls (summary) and short fixes

- Conflicts during pull/rebase: resolve files, add, and continue rebase/merge.
- Local branch differs from remote (diverged): fetch & rebase/merge, then push.
- Pre-commit hooks block commit: run the hook locally, fix issues, re-run commit.
- CI failures after PR: fix the failing tests or lint errors, push new commits to the PR branch.
- Force pushing: avoid unless absolutely necessary. If used, prefer `--force-with-lease`.

---

End of log.
