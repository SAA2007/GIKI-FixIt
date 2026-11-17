# GIKI FixIt — Team Development Plan & Architecture

**Course:** CS-101 (Computing & AI) — Semester Project  
**Team:** Syed Abdullah Ahmed (2026718), Syed Ibrahim Kashif (2026727), Momin Isam (2026374)  
**Workflow:** Turn-based rotation (one coder drives & pushes while the other two review/supervise, then rotate). All team members push commits to their own GitHub accounts across multiple days.

---

## 1. Project Folder Structure

All C++ code stays in **one file (`main.cpp`)** to keep compilation simple and avoid merge conflicts, supported by **3 CSV database files**:

```text
GIKI-FixIt/
 ├── main.cpp              # Complete C++ application (built in 6 turns)
 ├── accounts.csv          # Student, Worker, and Admin profiles
 ├── tasks.csv             # Active and completed (history) maintenance tasks
 ├── complaints.csv        # Student feedback complaints for Admin review
 └── DEVELOPMENT_PLAN.md   # This guide
```

---

## 2. CSV Database Schema, History & Time/Date Logic

### Why Only 3 CSV Files? (No Separate History File Needed)
We do not need a separate file for "History." Every task lives in `tasks.csv`, and its `Status` column tracks its lifecycle:
- `Pending` → Created by a Student, waiting for a Worker
- `In-Progress` → Picked up by a Worker
- `Completed` → Finished by the Worker

When a Student or Worker opens **"View Past / Completed Entries"**, the program simply loops through the task arrays and displays rows where `Status == "Completed"`.

### Table Schemas

| File | Purpose | Columns |
| :--- | :--- | :--- |
| **`accounts.csv`** | Stores all Student, Worker, and Admin accounts | `Username, EncryptedPassword, Role, RegNo_or_Skill, Hostel, Room, IsApproved, TasksDone, RatingPoints, Payout` |
| **`tasks.csv`** | Stores all maintenance tasks (active + history) | `TaskID, StudentUser, Hostel, Room, TaskType, Description, Urgency, Status, WorkerUser, StartTime, AllowedHours, EndTime` |
| **`complaints.csv`** | Stores student feedback complaints sent to Admin | `ComplaintID, StudentUser, TaskID, FeedbackText, DateSubmitted, Status` |

### How Date & Time Work (`#include <ctime>`)
- C++ has a built-in `time(0)` function that returns the current timestamp in seconds as an integer.
- **Assigned Completion Duration:** Calculated automatically from urgency (`1` to `5`):
  $$\text{Allowed Hours} = (6 - \text{Urgency}) \times 12$$
  *(Example: Urgency 5 gives 12 hours; Urgency 1 gives 60 hours.)*
- **On-Time Check:** When a Worker selects a task, we store `StartTime = time(0)`. When they mark it `Completed`, we store `EndTime = time(0)` and calculate:
  $$\text{Hours Taken} = \frac{\text{EndTime} - \text{StartTime}}{3600}$$
  If $\text{Hours Taken} \le \text{Allowed Hours}$, the task was completed on time.

---

## 3. Turn-by-Turn Development Plan (6 Turns = 2 Commits Each)

Each person completes one module at a time, tests that `main.cpp` compiles cleanly, commits & syncs via VS Code, and hands off to the next teammate.

### Turn 1 — Abdullah (Auth, Encryption & Accounts Database)
* **Focus Files:** `main.cpp` + `accounts.csv`
* **Features Covered:** Feature #1 (Signup & Sign-in) + Feature #2 (Password Encryption)
* **Tasks:**
  1. Declare global parallel arrays (`usernames[]`, `passwords[]`, `roles[]`, etc.) with fixed capacities.
  2. Write `encryptPassword()` using **Recursion** (shifts each character by a fixed offset).
  3. Write `saveAccounts()` and `loadAccounts()` to read/write `accounts.csv`.
  4. Build `signUp()` (Student vs. Worker with `IsApproved = 0`) and `signIn()` (first Admin hardcoded as `admin` / `admin123`, routing users to Student/Worker/Admin menus).

### Turn 2 — Ibrahim (Task Creation, Pointers & 2D Hostel Grid)
* **Focus Files:** `main.cpp` + `tasks.csv`
* **Features Covered:** Feature #3 (Creating a New Task Entry)
* **Tasks:**
  1. Write `saveTasks()` and `loadTasks()` to read/write `tasks.csv`.
  2. Write `countWords(const char* desc)` using **Pointers** to enforce description word limits.
  3. Build `createTask()` in the Student UI:
     - Select Task Type (`1–5`), enter Description, and rate Urgency (`1–5`).
     - Compute `AllowedHours = (6 - Urgency) * 12` and record creation timestamp via `time(0)`.
     - Increment the **2D Hostel Array** `hostelTasks[hostel][type]++` and save the new row to `tasks.csv`.

### Turn 3 — Momin (Student Task History & Feedback Complaints)
* **Focus Files:** `main.cpp` + `complaints.csv`
* **Features Covered:** Feature #4 Part A (Student Views Task Progress) + Student Feedback Complaints
* **Tasks:**
  1. Write `viewStudentTasks()`: Loops through loaded task arrays and displays all tasks for the logged-in Student (`Pending`, `In-Progress`, and `Completed` history).
  2. Write `saveComplaints()` and `loadComplaints()` to read/write `complaints.csv`.
  3. Build `submitFeedbackComplaint()`: Lets a Student file a feedback complaint for Admin review and saves it to `complaints.csv`.

### Turn 4 — Ibrahim (Worker Portal, Rating & Payout Formula)
* **Focus Files:** `main.cpp` (updates `tasks.csv` & `accounts.csv`)
* **Features Covered:** Feature #4 Part B (Worker Task Flow) + Feature #5 (Worker Rating & Automatic Payout)
* **Tasks:**
  1. Build the **Worker UI**:
     - View `Pending` tasks, pick a task (`Status = "In-Progress"`, `StartTime = time(0)`).
     - Mark an `In-Progress` task as `"Completed"` (`EndTime = time(0)`) using a **Pointer** function `updateTaskStatus(string* statusPtr)`.
  2. Allow Students to rate completed tasks from `1–5` stars.
  3. Write `calculateWorkerPayout(int workerIdx, int &points, int &payout)` using **Pass by Reference (`&`)**:
     - $\text{Rating Points} = (\text{Tasks Completed} \times 10) + (\text{Sum of Star Ratings} \times 2) - (\text{Late Tasks} \times 5)$
     - $\text{Worker Payout} = (\text{Tasks Completed} \times 500) + (\text{Rating Points} \times 25)$

### Turn 5 — Abdullah (Admin Oversight, Approvals & Hostel Matrix)
* **Focus Files:** `main.cpp` (updates `accounts.csv` & `tasks.csv`)
* **Features Covered:** Feature #6 Part A (Admin Controls)
* **Tasks:**
  1. Write `adminApproveWorkers()`: Lists Workers with `IsApproved == 0` so Admin can approve/activate them.
  2. Write `adminManageTasks()`: Displays the **2D Hostel Summary Table** (`hostelTasks[13][5]`), lists all tasks from `tasks.csv`, and lets Admin modify or delete entries that violate guidelines.
  3. Write `adminCreateAdmin()`: Allows an existing Admin to register new Admin accounts in `accounts.csv`.

### Turn 6 — Momin (Admin Complaint Review, Payout Adjustments & UI Polish)
* **Focus Files:** `main.cpp` (updates `complaints.csv` & `accounts.csv`)
* **Features Covered:** Feature #6 Part B (Admin Feedback Review & Final Polish)
* **Tasks:**
  1. Write `adminViewComplaints()`: Displays all student feedback complaints loaded from `complaints.csv`.
  2. Write `adminModifyWorkerStats()`: Allows Admin to adjust a Worker's Rating Points or Payout in `accounts.csv` after reviewing feedback.
  3. Polish CLI menu headers/formatting across all three dashboards and capture final screenshots for Section 5 of the project report.

---

## 4. Features-to-Coding Matrix (CS-101 Rubric Alignment)

| Sr # | Feature Name | Programming Concepts Used | CSV Files Used | Arrays Created | Pointers / References | Functions Created |
| :-: | :--- | :--- | :-: | :-: | :-: | :-: |
| **1** | New User Signup & Existing User Sign-in | 1D Arrays, Functions, Pass by Reference, File I/O, If/Switch/Loops | 1 (`accounts.csv`) | 8 (1D account arrays) | 1 (`int &userCount`) | 4 (`signUp`, `signIn`, `saveAccounts`, `loadAccounts`) |
| **2** | Password Encryption | **Recursion**, Strings/Char Arrays, Functions | 1 (`accounts.csv`) | 1 (password array) | 1 (`string &password`) | 2 (`encryptPassword`, `decryptPassword`) |
| **3** | Creating a New Task Entry | **2D Array** (`hostelTasks`), **Pointers** (`countWords`), File I/O | 1 (`tasks.csv`) | 10 (1D task arrays + 1 **2D array**) | 2 (`const char* desc`, `int &taskCount`) | 4 (`createTask`, `countWords`, `saveTasks`, `loadTasks`) |
| **4** | Checking Progress of Task | Arrays, **Functions with Pointers**, File I/O, Loops | 2 (`tasks.csv`, `complaints.csv`) | 5 (task & complaint arrays) | 1 (`string* statusPtr`) | 5 (`viewStudentTasks`, `workerPickTask`, `updateTaskStatus`, `submitFeedbackComplaint`, `save/loadComplaints`) |
| **5** | Worker Rating & Automatic Payout | **Pass by Value & Reference**, Math Formulas, File I/O | 2 (`tasks.csv`, `accounts.csv`) | 3 (rating & payout arrays) | 2 (`int &points`, `int &payout`) | 2 (`rateWorker`, `calculateWorkerPayout`) |
| **6** | Admin Profile & Management | **Functions with 2D Arrays**, Pointers, Switch/Loops, File I/O | 3 (All 3 CSVs) | All arrays | 2 (`bool* approvedPtr`, `int &count`) | 5 (`adminApproveWorkers`, `adminManageTasks`, `adminCreateAdmin`, `adminViewComplaints`, `adminModifyWorkerStats`) |
