cmpwi r3, 0x3
bge @80077E78
cmpwi r3, 0x0
bge @80077E64
b @80077E78
@80077E64
mulli r4, r3, 0x54
lis r3, lbl_80268940@ha
addi r0, r3, lbl_80268940@l
add r3, r0, r4
blr
@80077E78
li r3, 0x0
blr
