    nofralloc
    lis r4, lbl_80401C10@ha
    lwzu r0, lbl_80401C10@l(r4)
    li r5, 0x0
    cmpwi r0, 0x0
    bne @L_800F80C8
    b @L_800F80FC
    @L_800F80C8:
    lwzu r0, 0x6c(r4)
    cmpwi r0, 0x0
    bne @L_800F80D8
    b @L_800F80FC
    @L_800F80D8:
    lwzu r0, 0x6c(r4)
    cmpwi r0, 0x0
    bne @L_800F80E8
    b @L_800F80FC
    @L_800F80E8:
    lwzu r0, 0x6c(r4)
    cmpwi r0, 0x0
    bne @L_800F80F8
    b @L_800F80FC
    @L_800F80F8:
    mr r4, r5
    @L_800F80FC:
    cmplwi r4, 0x0
    bne @L_800F810C
    li r3, 0x4
    blr
    @L_800F810C:
    cmpwi r3, 0x5
    bge @L_800F8128
    cmpwi r3, 0x1
    bge @L_800F8120
    b @L_800F8128
    @L_800F8120:
    stw r3, 0x0(r4)
    b @L_800F8130
    @L_800F8128:
    li r3, 0x1
    blr
    @L_800F8130:
    li r3, 0x0
    blr
