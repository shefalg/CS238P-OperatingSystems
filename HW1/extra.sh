#!/bin/bash
echo "Before starting subshell"
echo "Hello 1";echo "Hello 2"
(echo "Hello inside inner subshell")
{ sleep 5;echo Waking up after 5 seconds;} &
{ sleep 1; echo Waking up after 1 second;} &
wait
echo All jobs are done!
