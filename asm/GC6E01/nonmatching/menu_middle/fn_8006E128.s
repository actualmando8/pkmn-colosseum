cmplwi r3, 0x0
bne @8006E138
li r3, 0x0
blr
@8006E138
lwz r5, 0x1c(r3)
cmplwi r5, 0x7
blt @8006E14C
li r3, 0x0
blr
@8006E14C
addi r4, r5, 0x1
slwi r0, r5, 2
stw r4, 0x1c(r3)
lwzx r3, r3, r0
blr
