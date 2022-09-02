#!/bin/bash

is_windows() {
	if [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
		false
	else
        true
	fi
}

is_nix() {
	if is_windows; then
		true
	else
		false
	fi
}

is_mac() {
	[[ $OSTYPE =~ .*darwin* ]] && true || false
}

is_linux() {
    [[ $OSTYPE =~ .*linux* ]] && true || false
}
