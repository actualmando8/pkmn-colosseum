stwu r1, -0xc20(r1)
mflr r0
stw r0, 0xc24(r1)
stw r31, 0xc1c(r1)
stw r30, 0xc18(r1)
stw r29, 0xc14(r1)
lis r3, lbl_80268AD0@ha
lfs f1, lbl_8047C108@sda21(r0)
addi r7, r3, lbl_80268AD0@l
li r31, 0x0
lwz r6, 0x0(r7)
li r3, 0x3
lwz r5, 0x4(r7)
lwz r4, 0x8(r7)
lwz r0, 0xc(r7)
stw r6, 0x8(r1)
stw r5, 0xc(r1)
stw r4, 0x10(r1)
stw r0, 0x14(r1)
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
li r3, 0xe1
bl fn_80102510
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1000
bl fn_800F9318
cmplwi r3, 0x0
beq @80079358
li r4, 0x0
bl fn_800E4014
@80079358
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1001
bl fn_800F9318
cmplwi r3, 0x0
beq @80079378
li r4, 0x0
bl fn_800E4014
@80079378
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1002
bl fn_800F9318
cmplwi r3, 0x0
beq @80079398
li r4, 0x0
bl fn_800E4014
@80079398
lis r3, 0xffe
addi r3, r3, 0x1000
bl fn_801CBA0C
mr r30, r3
bl fn_80113F48
mr r4, r30
bl fn_800F9318
lis r4, 0xfff
li r3, 0x5d4
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x1
bl fn_80176E0C
li r3, 0x4
bl fn_80177A44
lfs f1, lbl_8047C108@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
addi r0, r1, 0xf0
addi r5, r1, 0x8
stw r0, 0xc(r1)
addi r6, r1, 0x18
li r3, 0x0
li r4, 0x0
bl fn_800849B4
cmpwi r3, 0x0
bge @800794C0
li r0, 0x1
lfs f1, lbl_8047C108@sda21(r0)
stw r0, lbl_8047A638@sda21(r0)
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
mr r3, r30
bl fn_801CB9D8
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1000
bl fn_800F9318
cmplwi r3, 0x0
beq @80079450
li r4, 0x1
bl fn_800E4014
@80079450
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1001
bl fn_800F9318
cmplwi r3, 0x0
beq @80079470
li r4, 0x1
bl fn_800E4014
@80079470
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1002
bl fn_800F9318
cmplwi r3, 0x0
beq @80079490
li r4, 0x1
bl fn_800E4014
@80079490
bl fn_80113F48
lis r4, 0x1094
li r5, 0x0
addi r4, r4, 0x1800
li r6, 0x0
bl fn_80176E0C
lfs f1, lbl_8047C108@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
b @800798CC
@800794C0
lfs f1, lbl_8047C108@sda21(r0)
li r3, 0x3
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
mr r3, r30
bl fn_801CB9D8
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1000
bl fn_800F9318
cmplwi r3, 0x0
beq @800794FC
li r4, 0x1
bl fn_800E4014
@800794FC
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1001
bl fn_800F9318
cmplwi r3, 0x0
beq @8007951C
li r4, 0x1
bl fn_800E4014
@8007951C
bl fn_80113F48
lis r4, 0x104e
addi r4, r4, 0x1002
bl fn_800F9318
cmplwi r3, 0x0
beq @8007953C
li r4, 0x1
bl fn_800E4014
@8007953C
bl fn_80113F48
lis r4, 0x1094
li r5, 0x0
addi r4, r4, 0x1800
li r6, 0x0
bl fn_80176E0C
lfs f1, lbl_8047C108@sda21(r0)
li r3, 0x2
bl fn_801C41C8
li r3, 0x1
bl fn_801C40F0
lwz r0, 0x20(r1)
li r3, 0x0
lwz r4, 0x1c(r1)
rlwinm r10, r0, 0, 29, 29
rlwinm r8, r0, 0, 30, 30
clrlwi r6, r0, 31
lwz r0, 0x18(r1)
neg r9, r10
neg r7, r8
neg r5, r6
stw r0, lbl_8047A62C@sda21(r0)
or r0, r5, r6
or r9, r9, r10
or r7, r7, r8
stw r4, lbl_8047A628@sda21(r0)
srwi r6, r9, 31
srwi r0, r0, 31
srwi r5, r7, 31
stb r6, lbl_8047A635@sda21(r0)
stb r5, lbl_8047A634@sda21(r0)
stb r0, lbl_8047A633@sda21(r0)
bl fn_80079EF4
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800798CC
lbz r0, lbl_8047A632@sda21(r0)
li r29, 0x1
cmplwi r0, 0x0
beq @8007962C
addi r30, r1, 0x28
lhz r4, 0x26(r1)
mr r3, r30
li r5, 0x47
li r6, 0x3e7
bl fn_80029850
cmpwi r3, 0x1
bge @80079604
li r29, 0x0
b @8007962C
@80079604
lhz r4, 0x26(r1)
mr r3, r30
li r5, 0x47
li r6, 0x1
li r7, -0x1
li r8, 0x3e7
bl fn_800298DC
lwz r0, 0x20(r1)
ori r0, r0, 0x4
stw r0, 0x20(r1)
@8007962C
lbz r0, lbl_8047A630@sda21(r0)
cmplwi r0, 0x0
beq @80079658
lhz r4, 0x26(r1)
addi r3, r1, 0x28
li r5, 0x1
li r6, 0x3e7
bl fn_80029850
cmpwi r3, 0x1
bge @80079658
li r29, 0x0
@80079658
lbz r0, lbl_8047A631@sda21(r0)
li r30, 0x1
cmplwi r0, 0x0
beq @80079678
lhz r3, 0x24(r1)
subi r0, r3, 0x6
srwi r0, r0, 31
mr r30, r0
@80079678
addi r3, r1, 0xf0
li r4, 0x1
li r5, 0x0
bl fn_8012A5B0
mr r6, r3
mr r4, r29
mr r5, r30
li r3, 0x0
bl fn_80079C1C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @800798CC
li r3, 0x2
li r4, 0x3d3b
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lbz r0, lbl_8047A631@sda21(r0)
cmplwi r0, 0x0
beq @80079774
li r29, 0x0
b @800796E4
@800796D0
mr r4, r29
addi r3, r1, 0xf0
bl fn_8012AC08
bl fn_80124A60
addi r29, r29, 0x1
@800796E4
clrlwi r0, r29, 16
cmplwi r0, 0x6
blt @800796D0
bl fn_80115BD8
bl fn_801159F0
mr r4, r3
addi r3, r1, 0xf0
bl fn_80130770
li r30, 0x0
b @80079740
@8007970C
mr r4, r30
addi r3, r1, 0xf0
bl fn_8012AC08
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8007973C
mr r4, r30
addi r3, r1, 0xf0
bl fn_8012AC08
mr r31, r3
b @8007974C
@8007973C
addi r30, r30, 0x1
@80079740
clrlwi r0, r30, 16
cmplwi r0, 0x6
blt @8007970C
@8007974C
cmplwi r31, 0x0
bne @80079768
lis r3, lbl_80268AE0@ha
li r4, 0x460
addi r3, r3, lbl_80268AE0@l
li r5, lbl_8047C10C@sda21
bl fn_80196E10
@80079768
lwz r0, 0x20(r1)
ori r0, r0, 0x2
stw r0, 0x20(r1)
@80079774
lbz r0, lbl_8047A630@sda21(r0)
cmplwi r0, 0x0
beq @800797A8
lhz r4, 0x26(r1)
addi r3, r1, 0x28
li r5, 0x1
li r6, 0x1
li r7, -0x1
li r8, 0x3e7
bl fn_800298DC
lwz r0, 0x20(r1)
ori r0, r0, 0x1
stw r0, 0x20(r1)
@800797A8
li r3, 0x1
bl fn_80093574
mr r5, r31
addi r4, r1, 0x18
li r3, 0x1
bl fn_80092C90
li r3, 0x1
bl fn_80093574
li r3, 0x1
bl fn_80093610
cmpwi r3, 0xc
beq @8007982C
li r3, 0x1
bl fn_80093698
li r3, 0x2
li r4, 0x3d85
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r3, 0xef
bl fn_80102510
li r0, 0x1
stw r0, lbl_8047A638@sda21(r0)
b @800798CC
@8007982C
li r3, 0x1
bl fn_80093698
li r3, 0x1
bl fn_801069FC
li r3, 0x43c5
li r4, 0x1
li r5, 0x0
bl fn_801067E8
bl fn_8001E184
extsb r0, r3
cmpwi r0, 0x0
beq @800798A0
bge @8007986C
cmpwi r0, -0x1
bge @80079874
b @800798A0
@8007986C
cmpwi r0, 0x2
bge @800798A0
@80079874
li r3, 0x43ca
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0xef
bl fn_80102510
li r0, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @800798CC
@800798A0
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r3, 0xef
bl fn_80102510
li r0, 0x1
stw r0, lbl_8047A638@sda21(r0)
@800798CC
lwz r0, 0xc24(r1)
lwz r31, 0xc1c(r1)
lwz r30, 0xc18(r1)
lwz r29, 0xc14(r1)
mtlr r0
addi r1, r1, 0xc20
blr
