    nofralloc
    lis r5, -32704
    lwzu r0, 7184(r5)
    li r6, 0x0
    cmpw r0, r3
    bne @L_800F7938
    b @L_800F796C
@L_800F7938:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7948
    b @L_800F796C
@L_800F7948:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7958
    b @L_800F796C
@L_800F7958:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F7968
    b @L_800F796C
@L_800F7968:
    mr r5, r6
@L_800F796C:
    cmplwi r5, 0x0
    bne @L_800F797C
    li r3, 0x0
    blr
@L_800F797C:
    cmpwi r4, 0x1
    bne @L_800F798C
    lbz r3, 0x5b(r5)
    blr
@L_800F798C:
    lbz r3, 0x29(r5)
    blr
