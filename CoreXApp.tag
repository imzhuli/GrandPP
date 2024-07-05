alias rebuild='python3 _script/build_service.py -x ../CoreX'
alias rebuild_release='python3 _script/build_service.py -r -x ../CoreX -d ../PP2DB'
alias qb='python3 _script/clean_install.py; cmake --build _build/cpp -- install -j 10'
alias rb='rebuild -j 24'


