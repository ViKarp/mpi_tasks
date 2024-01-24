# Honor per-interactive-shell startup file
if [ -f ~/.bashrc ]; then . ~/.bashrc; fi
HISTCONTROL=erasedups:ignorespace HISTSIZE= HISTFILESIZE=
shopt -s histverify histappend
alias rm='rm --preserve-root --one-file-system'
alias mkdir='mkdir -p'
alias ls='ls --color=auto --group-directories-first'
alias gdb='gdb -q --args'
alias ps='ps --ppid 2 -p 2 --deselect -o pid,user:20,args'
alias ip='ip -brief -color'
