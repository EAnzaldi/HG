# Git

## SSH
Using the SSH protocol, you can access your repositories on GitHub. When you connect via SSH, you authenticate using a private key file on your local machine without supplying your username and personal access token at each visit. You can also use an SSH key to sign commits.
Follow the [tutorial] (https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)

## Commands
- git clone https://github.com/EAnzaldi/HG.git 
Clone repository in your pc.
- git add MyBlend.blend  
Add file to the repository.
- git commit -m "This is my commit"  
Insert changes into the repository. Always specify the change in the comment.
git status
Lists all the files that have to be committed.
- git push  
Sends the committed changes to the remote repository.
- git pull  
Fetches changes on the remote rempository to your working directory.

## Workflow
1. Pull external changes
2. Change something
3. Commit your changes
4. Push

