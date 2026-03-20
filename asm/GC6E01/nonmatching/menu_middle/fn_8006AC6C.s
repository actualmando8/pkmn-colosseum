lwz r4, lbl_80478F20@sda21(r0)
clrlwi r3, r3, 16
lwz r0, 0x0(r4)
cmplw r0, r3
bgt @8006AC88
li r3, -0x1
blr
@8006AC88
cmpwi r3, 0x9
bge @8006ACA0
cmpwi r3, 0x1
beq @8006ACC4
bge @8006ACB4
b @8006ACC4
@8006ACA0
cmpwi r3, 0x30a
bge @8006ACC4
cmpwi r3, 0x308
bge @8006ACBC
b @8006ACC4
@8006ACB4
li r3, 0x1
blr
@8006ACBC
li r3, 0x2
blr
@8006ACC4
li r3, 0x0
blr
