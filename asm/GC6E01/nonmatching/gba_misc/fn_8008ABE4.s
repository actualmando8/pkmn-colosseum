lis r6, lbl_803FB318@ha
slwi r7, r3, 2
addi r0, r6, lbl_803FB318@l
lis r5, lbl_803FB308@ha
add r9, r0, r7
slwi r10, r3, 1
subi r9, r9, 0x4
addi r6, r5, lbl_803FB308@l
lwz r3, 0x0(r9)
li r5, lbl_8047A684@sda21
add r7, r6, r7
li r8, 0x0
add r6, r5, r10
li r0, lbl_8047A67C@sda21
add r5, r0, r10
stw r4, 0x0(r9)
stw r8, -0x4(r7)
sth r8, -0x2(r6)
sth r8, -0x2(r5)
blr
