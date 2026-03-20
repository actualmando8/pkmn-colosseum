stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r31, r3
clrlwi r0, r31, 24
li r30, 0x0
cmplwi r0, 0x6
bge @80097C18
li r3, 0x0
li r4, 0x2
bl fn_80129280
cmplwi r3, 0x0
beq @80097C18
clrlwi r4, r31, 24
bl fn_8012AC08
mr r30, r3
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80097C18
li r30, 0x0
@80097C18
cmplwi r30, 0x0
bne @80097C30
li r3, -0x1
b @80097CB8
b @80097C30
@80097C2C
bl fn_800F0308
@80097C30
bl fn_8010B560
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80097C2C
lis r3, lbl_803FB380@ha
li r4, 0x0
addi r3, r3, lbl_803FB380@l
li r5, 0x44
bl memset
lis r3, lbl_803FB380@ha
li r5, 0x11
addi r31, r3, lbl_803FB380@l
li r4, 0x0
li r0, -0x1
stb r5, 0x0(r31)
li r3, 0x39d
stw r4, 0x8(r31)
stw r30, 0xc(r31)
sth r4, 0x18(r31)
stw r4, 0x10(r31)
stw r4, 0x14(r31)
stw r0, 0x4(r31)
bl fn_800FF730
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x0(r3)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
beq @80097CB0
li r3, 0x0
li r4, 0x0
bl fn_8011288C
@80097CB0
bl fn_800F0308
lwz r3, 0x4(r31)
@80097CB8
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
