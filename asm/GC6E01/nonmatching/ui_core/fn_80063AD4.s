stwu r1, -0x60(r1)
mflr r0
stw r0, 0x64(r1)
stfd f31, 0x50(r1)
psq_st f31, 0x58(r1), 0, 0
stw r31, 0x4c(r1)
stw r30, 0x48(r1)
mr r31, r4
lbz r4, 0x8b(r3)
lis r5, 0x4330
lbz r0, 0x67(r31)
lis r3, 0x8102
lwz r8, lbl_8047BFC8@sda21(r0)
subi r7, r3, 0x7dfd
mullw r6, r4, r0
lwz r0, lbl_8047BFCC@sda21(r0)
stw r8, 0x8(r1)
li r3, 0x1
lfd f4, lbl_8047BFD8@sda21(r0)
stw r0, 0xc(r1)
mulhw r7, r7, r6
lbz r4, 0xb(r1)
lbz r0, 0xf(r1)
stw r5, 0x10(r1)
lfd f2, lbl_8047BFE0@sda21(r0)
stw r4, 0x1c(r1)
add r4, r7, r6
stw r5, 0x18(r1)
srawi r4, r4, 15
srwi r6, r4, 31
stw r0, 0x2c(r1)
add r0, r4, r6
lfd f0, 0x18(r1)
xoris r0, r0, 0x8000
stw r5, 0x28(r1)
fsubs f1, f0, f2
stw r0, 0x14(r1)
lfd f0, 0x28(r1)
lfd f3, 0x10(r1)
fsubs f0, f0, f2
fsubs f31, f3, f4
fmuls f1, f1, f31
fmuls f0, f0, f31
fctiwz f1, f1
fctiwz f0, f0
stfd f1, 0x20(r1)
stfd f0, 0x30(r1)
lwz r4, 0x24(r1)
lwz r0, 0x34(r1)
stb r4, 0xb(r1)
stb r0, 0xf(r1)
bl fn_800D88DC
li r3, 0x6
bl fn_800D888C
li r3, 0x6
bl fn_800D6A00
lis r3, lbl_80314E08@ha
addi r3, r3, lbl_80314E08@l
bl fn_800D7820
li r3, 0x4
bl fn_800D67BC
li r3, 0x0
li r4, 0x0
bl fn_800D61E4
lwz r4, 0x8(r1)
li r3, 0x0
bl fn_800D5BA0
lha r3, 0x54(r31)
li r4, 0x0
bl fn_800D61E4
lwz r4, 0x8(r1)
li r3, 0x0
bl fn_800D5BA0
lha r3, 0x54(r31)
lha r4, 0x56(r31)
bl fn_800D61E4
lwz r4, 0xc(r1)
li r3, 0x0
bl fn_800D5BA0
lha r4, 0x56(r31)
li r3, 0x0
bl fn_800D61E4
lwz r4, 0xc(r1)
li r3, 0x0
bl fn_800D5BA0
bl fn_800D6728
lha r5, 0x54(r31)
li r3, 0x0
lha r6, 0x56(r31)
li r4, 0x0
bl fn_800FE38C
li r3, 0x1
bl fn_800D88DC
li r3, 0x6
bl fn_800D888C
lfs f1, lbl_8047BFD0@sda21(r0)
bl fn_800D5648
li r3, 0x1
bl fn_800D6A00
lis r3, lbl_80314E08@ha
addi r3, r3, lbl_80314E08@l
bl fn_800D7820
lfs f0, lbl_8047BFD4@sda21(r0)
li r0, 0xff
stb r0, 0x8(r1)
li r30, 0x0
fmuls f0, f0, f31
stb r0, 0x9(r1)
fctiwz f0, f0
stb r0, 0xa(r1)
stfd f0, 0x38(r1)
lwz r0, 0x3c(r1)
stb r0, 0xb(r1)
b @80063CDC
@80063C9C
li r3, 0x2
bl fn_800D67BC
mr r4, r30
li r3, 0x0
bl fn_800D61E4
lwz r4, 0x8(r1)
li r3, 0x0
bl fn_800D5BA0
lha r3, 0x54(r31)
mr r4, r30
bl fn_800D61E4
lwz r4, 0x8(r1)
li r3, 0x0
bl fn_800D5BA0
bl fn_800D6728
addi r30, r30, 0x4
@80063CDC
lha r0, 0x56(r31)
extsh r3, r30
cmpw r3, r0
blt @80063C9C
bl fn_800FE35C
psq_l f31, 0x58(r1), 0, 0
lwz r0, 0x64(r1)
lfd f31, 0x50(r1)
lwz r31, 0x4c(r1)
lwz r30, 0x48(r1)
mtlr r0
addi r1, r1, 0x60
blr
