stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r30, r3
lwz r29, 0x0(r4)
addi r31, r4, 0x10
lwz r28, 0x4(r4)
li r5, 0x0
rlwinm r3, r29, 0, 16, 23
lwz r0, 0x8(r4)
rlwinm r10, r29, 0, 8, 15
slwi r9, r29, 24
slwi r7, r3, 8
rlwinm r8, r28, 0, 16, 23
lwz r3, 0xc(r4)
rlwinm r11, r28, 0, 8, 15
rlwinm r6, r0, 0, 16, 23
srwi r12, r10, 8
or r10, r9, r7
rlwinm r9, r0, 0, 8, 15
rlwinm r4, r3, 0, 16, 23
rlwinm r7, r3, 0, 8, 15
srwi r29, r29, 24
or r10, r12, r10
or r12, r29, r10
slwi r10, r28, 24
slwi r8, r8, 8
srwi r11, r11, 8
or r10, r10, r8
stw r12, 0x0(r30)
srwi r12, r28, 24
slwi r8, r0, 24
or r10, r11, r10
slwi r6, r6, 8
or r10, r12, r10
srwi r9, r9, 8
or r8, r8, r6
slwi r6, r3, 24
slwi r4, r4, 8
stw r10, 0x4(r30)
srwi r10, r0, 24
or r0, r9, r8
or r8, r10, r0
srwi r7, r7, 8
or r0, r6, r4
srwi r3, r3, 24
or r0, r7, r0
stw r8, 0x8(r30)
or r3, r3, r0
srwi r0, r3, 16
sth r0, 0xc(r30)
clrlwi r0, r3, 16
sth r0, 0xe(r30)
lhz r0, 0xe(r30)
cmplwi r0, 0x32
beq @80089478
cmplwi r0, 0x1e
bne @8008947C
@80089478
li r5, 0x1
@8008947C
cmpwi r5, 0x0
bne @8008949C
lis r3, lbl_8026F568@ha
lis r5, lbl_8026F574@ha
addi r3, r3, lbl_8026F568@l
li r4, 0x6f
addi r5, r5, lbl_8026F574@l
bl fn_80196E10
@8008949C
mr r6, r30
li r7, 0x0
b @800894EC
@800894A8
lwz r5, 0x0(r31)
addi r31, r31, 0x4
addi r7, r7, 0x1
rlwinm r0, r5, 0, 16, 23
rlwinm r4, r5, 0, 8, 15
slwi r3, r5, 24
srwi r5, r5, 24
slwi r0, r0, 8
srwi r4, r4, 8
or r0, r3, r0
or r0, r4, r0
or r3, r5, r0
clrlwi r0, r3, 16
sth r0, 0x10(r6)
srwi r0, r3, 16
sth r0, 0x12(r6)
addi r6, r6, 0x4
@800894EC
lhz r0, 0xe(r30)
cmpw r7, r0
blt @800894A8
lwz r0, lbl_8047A664@sda21(r0)
cmpwi r0, 0x0
beq @80089514
li r0, 0x0
stw r0, 0x0(r30)
stw r0, 0x4(r30)
stw r0, lbl_8047A664@sda21(r0)
@80089514
lwz r0, lbl_8047A660@sda21(r0)
cmpwi r0, 0x0
beq @80089548
lwz r4, 0x0(r30)
li r0, 0x0
lwz r3, lbl_8047A660@sda21(r0)
add r3, r4, r3
stw r3, 0x0(r30)
lwz r4, 0x4(r30)
lwz r3, lbl_8047A660@sda21(r0)
add r3, r4, r3
stw r3, 0x4(r30)
stw r0, lbl_8047A660@sda21(r0)
@80089548
lwz r0, lbl_8047A66C@sda21(r0)
cmpwi r0, 0x0
beq @80089560
li r0, 0x0
stw r0, 0x8(r30)
stw r0, lbl_8047A66C@sda21(r0)
@80089560
lwz r0, lbl_8047A668@sda21(r0)
cmpwi r0, 0x0
beq @80089580
lwz r3, 0x8(r30)
li r0, 0x0
ori r3, r3, 0x10
stw r3, 0x8(r30)
stw r0, lbl_8047A668@sda21(r0)
@80089580
li r3, 0x1
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
