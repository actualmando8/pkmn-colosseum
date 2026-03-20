stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r4
lha r0, 0x6(r29)
cmpwi r0, 0xd3d
beq @8007561C
bge @8007561C
cmpwi r0, 0xd3c
bge @80075550
b @8007561C
@80075550
lwz r3, lbl_8047A610@sda21(r0)
lfs f0, 0x18c(r3)
addi r3, r3, 0x144
fctiwz f0, f0
stfd f0, 0x8(r1)
lwz r31, 0xc(r1)
bl fn_80109934
mr r30, r3
cmplwi r30, 0x0
beq @8007561C
li r3, 0x3
bl fn_800D88DC
li r3, 0x4
bl fn_800D888C
li r3, 0x7
bl fn_800D6A00
lis r3, lbl_80314F98@ha
addi r3, r3, lbl_80314F98@l
bl fn_800D7820
mr r4, r30
li r3, 0x0
bl fn_800D85D4
li r3, 0x2
bl fn_800D67BC
li r3, 0x0
li r4, 0x0
bl fn_800D61E4
mr r7, r31
li r3, 0x0
li r4, 0x28
li r5, 0x3e
li r6, 0xc8
bl fn_800D5CB8
lfs f1, lbl_8047C0A8@sda21(r0)
li r3, 0x0
fmr f2, f1
bl fn_800D59B8
lha r3, 0x54(r29)
lha r4, 0x56(r29)
bl fn_800D61E4
mr r7, r31
li r3, 0x0
li r4, 0x28
li r5, 0x3e
li r6, 0xc8
bl fn_800D5CB8
lfs f1, lbl_8047C0AC@sda21(r0)
li r3, 0x0
fmr f2, f1
bl fn_800D59B8
bl fn_800D6728
@8007561C
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
