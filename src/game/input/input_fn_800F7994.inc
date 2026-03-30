    nofralloc
    lis r5, -32704
    lwzu r0, 7184(r5)
    li r6, 0x0
    cmpw r0, r3
    bne @L_800F79AC
    b @L_800F79E0
@L_800F79AC:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F79BC
    b @L_800F79E0
@L_800F79BC:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F79CC
    b @L_800F79E0
@L_800F79CC:
    lwzu r0, 0x6c(r5)
    cmpw r0, r3
    bne @L_800F79DC
    b @L_800F79E0
@L_800F79DC:
    mr r5, r6
@L_800F79E0:
    cmplwi r5, 0x0
    bne @L_800F79F0
    li r3, 0x0
    blr
@L_800F79F0:
    cmpwi r4, 0x1
    bne @L_800F7A00
    lbz r3, 0x5a(r5)
    blr
@L_800F7A00:
    lbz r3, 0x28(r5)
    blr
