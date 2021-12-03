#!/bin/bash

echo "testing hello"
./minishell "hello" | grep -a "OK\|ERROR"
echo "testing hello > g"
./minishell "hello > g" | grep -a "OK\|ERROR"
echo "testing hello < g"
./minishell "hello < g" | grep -a "OK\|ERROR"
echo "testing hello || hewwo"
./minishell "hello || hewwo" | grep -a "OK\|ERROR"
echo "testing hello | hewwo"
./minishell "hello | hewwo" | grep -a "OK\|ERROR"