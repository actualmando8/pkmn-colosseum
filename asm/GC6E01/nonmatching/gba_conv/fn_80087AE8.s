stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
mr r30, r3
mr r26, r4
li r0, 0x0
li r31, 0x1
stw r0, 0x28(r30)
lbz r0, 0x21(r30)
extsb r3, r0
cmpwi r3, 0x0
blt @80087B30
bl fn_800D0F44
subis r0, r3, 0x4
cmplwi r0, 0x0
bne @80087B30
li r31, 0x0
@80087B30
rlwinm r28, r26, 0, 30, 30
clrlwi r27, r26, 31
rlwinm r26, r26, 0, 29, 29
@80087B3C
li r3, 0x10c
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80087B58
bl fn_800F0308
b @80087B3C
@80087B58
cmpwi r28, 0x0
beq @80087B84
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 26, 26
cmpwi r0, 0x0
beq @80087B84
li r0, 0x2
li r3, 0x0
stw r0, 0x28(r30)
b @80087C50
@80087B84
lwz r0, 0x28(r30)
cmplwi r0, 0x8
bne @80087B98
li r3, 0x0
b @80087C50
@80087B98
cmpwi r27, 0x0
beq @80087BF4
bl fn_80106934
mr r29, r3
extsb r0, r29
cmpwi r0, 0x0
bne @80087BC4
li r0, 0x1
li r3, 0x1
stw r0, 0x28(r30)
b @80087C50
@80087BC4
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 27, 27
cmpwi r0, 0x0
beq @80087BF4
extsb r0, r29
cmpwi r0, -0x1
bne @80087BF4
li r0, 0x1
li r3, 0x1
stw r0, 0x28(r30)
b @80087C50
@80087BF4
cmpwi r26, 0x0
beq @80087C48
lbz r3, 0x21(r30)
extsb r3, r3
bl fn_800D0F44
cmplwi r3, 0x80
beq @80087C48
clrlwi r0, r31, 24
cmplwi r0, 0x0
beq @80087C38
subis r0, r3, 0x4
cmplwi r0, 0x0
bne @80087C48
li r0, 0x4
li r3, 0x1
stw r0, 0x28(r30)
b @80087C50
@80087C38
subis r0, r3, 0x4
cmplwi r0, 0x0
beq @80087C48
li r31, 0x1
@80087C48
bl fn_800F0308
b @80087B3C
@80087C50
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
