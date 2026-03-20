lis r4, lbl_803FB318@ha
slwi r5, r3, 2
addi r0, r4, lbl_803FB318@l
li r4, 0x0
add r3, r0, r5
lwz r0, -0x4(r3)
cmpwi r0, 0x0
beq @8008ABDC
lis r3, lbl_803FB308@ha
addi r0, r3, lbl_803FB308@l
add r3, r0, r5
lwz r0, -0x4(r3)
cmpwi r0, 0x0
bne @8008ABDC
li r4, 0x1
@8008ABDC
clrlwi r3, r4, 24
blr
