# 如何编译

## 依赖

```text
1. boost
    ubuntu 安装
    sudo apt-get install libboost-all-dev
2. z3 lib
    下载链接: https://github.com/Z3Prover/z3/releases/tag/z3-4.6.0
    将libz3.so放到 $(BASEDIR)/lib 下
```

## 编译

```text
1. 修改Makefile.properties文件
    BASEDIR=当前工程所在目录(comspen)
2. make
    生成二进制文件CompSpen
3. make clean
    清除构建
```

## 测试用例

```text
samples文件夹下
    qf_shls_entl
    qf_shls_sat
    QF_SLID_LC_ENTL
    QF_SLID_LC_SAT

运行示例:
./CompSpen samples/QF_SLID_LC_SAT/dll-sat-01.smt 
Checking satisfiability.
The result: sat

Total time (sec): 0.008694

注意: 运行前运行
source env.sh
```