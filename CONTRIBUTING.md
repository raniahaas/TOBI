Further development and contributions to TOBI and further UCRC projects will often follow the same structure for code development. We encourage you to play around with your own custom code on **both** the hardware and software side to see how it plays out before committing and testing to an open issue.
# Following a new Issue
<img width="1505" height="730" alt="Issues Tab for the TOBI repository" src="https://github.com/user-attachments/assets/f5e9d932-949f-452d-a2bd-0423ed30d690" />
Under the issues tab, there will often be several tickets for items that need work on. Clicking on each ticket will give you a more in-depth description of the issue at hand, possible methods of executions, considerations for implementation, and members feedback and communcation on issues in development. Please comment and signify if you are picking up a ticket so it can properly be assigned to you, letting other members know who is working on what code. Often, each ticket will come with a branch attached to it, however if there is no branch you can 'Create a branch' for development designated directly to a ticket. Find a new issue within the ticket that is creating a larger scope than intended? Create a sub-ticket and signify the additional focus and add a new branch

# Documenting code
There are two main locations for documenting your code: headers and modified locations. At the top of all files are headers that look like such and include date, user, and what modifications were made.
``` 
/** 07-14-2026 - RH - Modified main.cpp to include data logging for barometer and accelerometer
*/
```
This is great for helping keeping track of who did what work and also gives you credit to everyone else when you make a great function! Additionally when modifiying large blocks of code often in main.cpp or other commonly shared files, indicate your **START** & **STOP** locations in case we need to roll back code, we can easily identify where issues are possibly occuring from. In files that are either custom to your ticket or are short in nature, simple one or two line comments about the lines added in addition to the headers are fine. Addiitional comments to what functions do are not required in START/STOP scenarios if they are not needed.
```
void setup() {
  Serial.begin(115200);
  delay(3000);

  // RH - BEGIN
  //Start reading from SDA and SCL ports
  Wire.begin(5, 6);
  //RH - END
}
```

# Pushing code
<img width="964" height="666" alt="GitHub Desktop app" src="https://github.com/user-attachments/assets/01e24ff2-76f4-47f5-a11f-ccdb37756821" />
For good practice, it is always a good idea when you are working on a project to push your code after you accomplish a part, whether big or small. Add your commit message which is a general overall of what you had worked on as well as a description where you can indicate what you worked on, what still needs to be worked on, and what still may be need additional research or testing. Make sure to push to origin once you push locally.

# Unit testing

# Peer Reviewing coding

# Rebasing code
Rebasing code essentially allows you once a ticket is finished to close the ticket and merge the code with our main branch (remember, main is always the most updated code, this is what we are running launch day on!). Ensure that you have all of your code peer reviewed and checked with leads before finally pusing and rebasing.
```
git checkout main
git pull
git checkout branch-name
git rebase main
```
