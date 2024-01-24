# Bash initialization for interactive non-login shells and
# for remote shells (info "(bash) Bash Startup Files").
# 'Export 'SHELL' to child processes.  Programs such as 'screen'
# honor and otherwise use /bin/sh.
export SHELL
if [[ $- != *i* ]]
then
    # We are being invoked from a non-interactive shell.  If this
    # is an SSH session (as in "ssh host command"), source
    # /etc/profile so we get PATH and other essential variables.
    [[ -n "$SSH_CLIENT" ]] && source /etc/profile

    # Don't do anything else.
    return
fi

# Source the system-wide file.
source /etc/bashrc

# Adjust the prompt depending on whether we're in 'guix environment'.
if [ -n "$GUIX_ENVIRONMENT" ]
then
    PS1='\u@\h \w [env]\$ '
else
    PS1='\u@\h \w\$ '
fi
alias ls='ls -p --color=auto'
alias ll='ls -l'
alias grep='grep --color=auto'
HISTCONTROL=erasedups:ignorespace HISTSIZE= HISTFILESIZE=
shopt -s histverify histappend
alias rm='rm --preserve-root --one-file-system'
alias mkdir='mkdir -p'
alias ls='ls --color=auto --group-directories-first'
alias gdb='gdb -q --args'
alias ps='ps --ppid 2 -p 2 --deselect -o pid,user:20,args'
alias ip='ip -brief -color'
module add hpc-2022
