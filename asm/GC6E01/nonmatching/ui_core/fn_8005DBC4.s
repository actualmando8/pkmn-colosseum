stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
li r3, 0xbb
bl fn_80102620
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8005DBF0
li r3, 0xbb
bl fn_80102510
b @8005DC10
@8005DBF0
li r3, 0xbb
li r4, 0x0
li r5, 0x0
li r6, 0x0
li r7, 0x1
li r8, 0x0
crclr 6
bl fn_801026A4
@8005DC10
li r3, 0x0
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
