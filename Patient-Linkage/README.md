MPC-GMW
===

Compile MPC-GMW runtime
---

```bash
make
```

Run your code
---

```bash
 #debug your code
 ./tt_sh/run.sh 4 config_p2p
```

```bash
 ./tt_sh/config_N_run.sh 8 100
 #tt_input
```

```bash
 #benchmarking p2p
 ./tt_sh/config_N_run-p2p.sh 8 100
```

```bash
 #benchmarking topk based on hamming distance
 ./tt_sh/config_N_run-topkhamm.sh 4 10
```
 
```bash
 #benchmarking topk based on set union
 ./tt_sh/config_N_run-setunion.sh 4 10
```

```bash
 #bechmarking hamming distance (one to one) max lines 250:
./tt_sh/config_N_run-hamming.sh input_dir/hu604D39.snp input_dir/hu661AD0.snp 250
 #by Jain
```

Code modification from GMW runtime
---

```
==============EDIT START==========================
in editable/circuit/circuit.cpp, add the following

	else if( name == "topk-hamm" )
	{
		pCircuit = new CHammDist();
	}
	else if( name == "set-intersection" )
	{
		pCircuit = new CSICircuit();
	}

add src files:
    editable/circuit/grouping.cpp
    editable/circuit/grouping.h
    editable/circuit/hammdist.cpp
    editable/circuit/hammdist.h
==============EDIT END============================
```


Git repository setup
===

Passphrase-less ssh with git
---

In the bit-bucket website, under ``manage account``, set ssh-keys by copying it from:

```bash
$ ssh-keygen -t rsa #if you have not created one yet
$ cat ~/.ssh/id_rsa.pub
```
 
Here is the link pattern to account manager: ``https://bitbucket.org/account/user/youraccount/ssh-keys/``

Set up your local directory
---

Set up Git on your machine if you haven't already.

You can simply do a ``clone``:

```bash
$ mkdir /path/to/your/project
$ cd /path/to/your/project
$ git clone git@bitbucket.org:tristartom/mpc-gmw.git
```

OR you can ``init`` and ``pull``:

```bash
$ mkdir /path/to/your/project
$ cd /path/to/your/project
$ git init
$ git remote add origin git@bitbucket.org:tristartom/mpc-gmw.git
$ git pull -u origin master # pull all updates from master branch
```

Make your first file update, commit, and push
---

```bash
$ echo "Your Name" >> contributors.txt
$ git add -u -A #update all file moves
$ git commit -am 'Initial commit with contributors'
$ git push -u origin master
```

Pull (remote) updates to your local repos
---
```bash
$ git pull -u origin master 
```

Multi-branch workflow
---
The following consider a user: ``scott``. For users other than scott, you should pick YOUR OWN NICKNAME.

#### Create your default branch

```
$ git push origin master:dev-scott #<remote-name> <local-branch-name>:<remote-branch-name>
```

#### Use/set up your default branch

```
$ git init
$ git remote add origin git@bitbucket.org:tristartom/tracing.git
$ git pull -u origin dev-scott # pull all updates from branch dev-scott and other branches
$ #initially you are on the master branch
$ git branch dev-scott #create a branch named dev-scott (if none) from master branch
$ git checkout dev-scott
$ git config --global push.default current
```

#### Make your first file update, commit, and push

```bash
$ echo "scott" >> contributors.txt
$ git add -A #update all file moves
$ git commit -am 'Initial commit with contributors from scott'
$ git push 
$ # or more specifically: git push -u origin dev-su
```

#### Pull (remote) updates to your local repos

```bash
$ git pull -u origin dev-scott
```
