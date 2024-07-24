alias rebuild='python3 _script/build_service.py -x ../CoreX'
alias rebuild_release='python3 _script/build_service.py -r -x ../CoreX -d ../PP2DB'
alias qc='python3 _script/clean_install.py;'
alias qb='qc; cmake --build _build/cpp -j 10'
alias qi='qb; cmake --build _build/cpp -- install -j 10'
alias rb='rebuild -j 24'


