# Git

## SSH
Using the SSH protocol, you can access your repositories on GitHub. When you connect via SSH, you authenticate using a private key file on your local machine without supplying your username and personal access token at each visit. You can also use an SSH key to sign commits.
Follow the [tutorial] (https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)

## Commands
- git init  
Inizialize the repository.
- git clone https://github.com/EAnzaldi/HG.git  
Clone repository in your pc.
- git add MyBlend.blend  
Add file to the repository.
- git commit -m "I have done this..."  
Insert changes into the repository. Always explain the changes in the comment.
- git status  
Displays the state of the working area.
- git push  
Sends the committed changes to the remote repository.
- git pull  
Fetches changes on the remote rempository to your working directory.

## Workflow
1. Pull external changes
2. Change something
4. Verify status
3. Commit your changes
4. Push

