    nofralloc
    cmpwi r3, 0x3
    bge @L_80077E78
    cmpwi r3, 0x0
    bge @L_80077E64
    b @L_80077E78
@L_80077E64:
    mulli r4, r3, 0x54
    lis r3, lbl_80268940@ha
    addi r0, r3, lbl_80268940@l
    add r3, r0, r4
    blr
@L_80077E78:
    li r3, 0x0
    blr
