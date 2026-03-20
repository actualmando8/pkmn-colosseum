stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0x8ae
bl fn_801906A0
cmplwi r3, 0x0
beq @8006AED8
li r3, 0x0
li r4, 0xe
bl fn_80129280
addis r3, r3, 0x1
lbz r0, -0x3678(r3)
cmplwi r0, 0x0
beq @8006AE58
subi r3, r3, 0x4cd8
b @8006AE5C
@8006AE58
li r3, 0x0
@8006AE5C
cmplwi r3, 0x0
beq @8006AED8
lhz r0, 0x0(r3)
cmpwi r0, 0x3
beq @8006AEAC
bge @8006AE84
cmpwi r0, 0x1
beq @8006AE9C
bge @8006AEA4
b @8006AEC4
@8006AE84
cmpwi r0, 0x309
beq @8006AEB4
bge @8006AEC4
cmpwi r0, 0x308
bge @8006AEBC
b @8006AEC4
@8006AE9C
li r3, 0x0
b @8006AEDC
@8006AEA4
li r3, 0x1
b @8006AEDC
@8006AEAC
li r3, 0x2
b @8006AEDC
@8006AEB4
li r3, 0x3
b @8006AEDC
@8006AEBC
li r3, 0x4
b @8006AEDC
@8006AEC4
lis r3, lbl_80267DE8@ha
li r4, 0x1c2
addi r3, r3, lbl_80267DE8@l
li r5, lbl_8047C040@sda21
bl fn_80196E10
@8006AED8
li r3, 0x0
@8006AEDC
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
