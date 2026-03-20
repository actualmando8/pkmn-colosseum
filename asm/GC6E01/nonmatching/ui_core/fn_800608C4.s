stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r4
lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
addi r3, r3, 0x36c
bl fn_80109934
mr r31, r3
cmplwi r31, 0x0
beq @8006099C
li r3, 0x3
bl fn_800D88DC
li r3, 0x4
bl fn_800D888C
li r3, 0x7
bl fn_800D6A00
lis r3, lbl_80314F98@ha
addi r3, r3, lbl_80314F98@l
bl fn_800D7820
mr r4, r31
li r3, 0x0
bl fn_800D85D4
li r3, 0x2
bl fn_800D67BC
li r3, 0x0
li r4, 0x0
bl fn_800D61E4
li r3, 0x0
li r4, 0xff
li r5, 0xff
li r6, 0xff
li r7, 0xff
bl fn_800D5CB8
lfs f1, lbl_8047BF60@sda21(r0)
li r3, 0x0
fmr f2, f1
bl fn_800D59B8
lha r3, 0x54(r30)
lha r4, 0x56(r30)
bl fn_800D61E4
li r3, 0x0
li r4, 0xff
li r5, 0xff
li r6, 0xff
li r7, 0xff
bl fn_800D5CB8
lfs f1, lbl_8047BF90@sda21(r0)
li r3, 0x0
fmr f2, f1
bl fn_800D59B8
bl fn_800D6728
@8006099C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
