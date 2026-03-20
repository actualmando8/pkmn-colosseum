stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r28, r3
mr r29, r4
lis r4, lbl_80267DD8@ha
mr r3, r29
addi r31, r4, lbl_80267DD8@l
li r30, 0x1
bl fn_8012A784
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006A8B0
bge @8006A878
cmpwi r0, 0x0
bge @8006A884
b @8006A950
@8006A878
cmpwi r0, 0x3
bge @8006A950
b @8006A900
@8006A884
mr r3, r29
li r30, 0x1
bl fn_8012AA2C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006A960
addi r3, r31, 0x10
addi r5, r31, 0x20
li r4, 0x281
bl fn_80196E10
b @8006A960
@8006A8B0
mr r3, r29
bl fn_8012AA2C
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006A8E4
bge @8006A8D4
cmpwi r0, 0x0
bge @8006A8DC
b @8006A8EC
@8006A8D4
cmpwi r0, 0x3
b @8006A8EC
@8006A8DC
li r30, 0x2
b @8006A960
@8006A8E4
li r30, 0x3
b @8006A960
@8006A8EC
addi r3, r31, 0x10
addi r5, r31, 0x4c
li r4, 0x28a
bl fn_80196E10
b @8006A960
@8006A900
mr r3, r29
bl fn_8012AA2C
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006A934
bge @8006A924
cmpwi r0, 0x0
bge @8006A92C
b @8006A93C
@8006A924
cmpwi r0, 0x3
b @8006A93C
@8006A92C
li r30, 0x309
b @8006A960
@8006A934
li r30, 0x308
b @8006A960
@8006A93C
addi r3, r31, 0x10
addi r5, r31, 0x4c
li r4, 0x294
bl fn_80196E10
b @8006A960
@8006A950
addi r3, r31, 0x10
addi r5, r31, 0x60
li r4, 0x299
bl fn_80196E10
@8006A960
mr r3, r28
mr r4, r29
mr r5, r30
bl fn_8006A990
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
