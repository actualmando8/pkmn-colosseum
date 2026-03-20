stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
li r3, 0xca
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8005DCAC
bl fn_800FF56C
mr r31, r3
bl fn_80117AD4
cmplw r31, r3
beq @8005DC74
mr r3, r31
bl fn_801176C8
li r0, 0x0
stw r0, lbl_8047A5B0@sda21(r0)
stw r0, lbl_8047A5B4@sda21(r0)
stw r0, lbl_8047A5B8@sda21(r0)
@8005DC74
li r0, 0x0
li r3, 0xca
stb r0, lbl_8047A5BC@sda21(r0)
li r4, 0x0
li r5, 0x0
li r6, 0x0
li r7, 0x1
li r8, 0x0
crclr 6
bl fn_801026A4
li r3, 0xca
li r4, 0xc
li r5, 0xa
bl fn_80102868
@8005DCAC
li r3, 0x0
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
