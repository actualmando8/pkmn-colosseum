stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r28, r3
lwz r0, lbl_8047BF48@sda21(r0)
stw r0, 0xc(r1)
bl fn_80105624
lhz r0, 0x4(r3)
rlwinm r0, r0, 0, 23, 23
cmpwi r0, 0x0
beq @8005DAB4
mr r3, r28
li r4, 0x0
bl fn_801040D0
addi r5, r3, 0x1
mr r3, r28
slwi r0, r5, 30
li r4, 0x0
srwi r5, r5, 31
subf r0, r5, r0
rotlwi r0, r0, 2
add r5, r0, r5
bl fn_801040B8
@8005DAB4
mr r3, r28
li r4, 0x0
bl fn_801040D0
cmpwi r3, 0x2
beq @8005DB08
bge @8005DADC
cmpwi r3, 0x0
beq @8005DAE8
bge @8005DAF8
b @8005DAE8
@8005DADC
cmpwi r3, 0x4
bge @8005DAE8
b @8005DB18
@8005DAE8
li r28, 0xa
li r29, 0x82
li r30, 0x145
b @8005DB24
@8005DAF8
li r28, 0xa
li r29, 0x82
li r30, 0x27
b @8005DB24
@8005DB08
li r28, 0x20
li r29, 0x1a0
li r30, 0x27
b @8005DB24
@8005DB18
li r28, 0x1
li r29, 0xd
li r30, 0x1ba
@8005DB24
li r3, 0x0
li r4, 0x7
li r5, 0x2
bl fn_800DA1E8
lwz r0, 0xc(r1)
subi r4, r30, 0x5
addi r6, r29, 0x12
addi r7, r1, 0x8
stw r0, 0x8(r1)
li r3, 0xf
li r5, 0x25d
bl fn_8001E58C
li r29, 0x0
lis r31, 0xc0c1
b @8005DB98
@8005DB60
bl fn_800DD384
add r0, r29, r3
subf r3, r28, r0
cmpwi r3, 0x0
blt @8005DB90
bl fn_800DD270
mr r6, r3
mr r4, r30
subi r5, r31, 0x3f01
li r3, 0x14
crclr 6
bl fn_800FAEF8
@8005DB90
addi r30, r30, 0xd
addi r29, r29, 0x1
@8005DB98
cmpw r29, r28
blt @8005DB60
li r3, 0x0
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
