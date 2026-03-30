    nofralloc
    lis r8, -32704
    lwzu r0, 7184(r8)
    li r9, 0x0
    cmpw r0, r3
    bne @L_800F78BC
    b @L_800F78F0
@L_800F78BC:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F78CC
    b @L_800F78F0
@L_800F78CC:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F78DC
    b @L_800F78F0
@L_800F78DC:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F78EC
    b @L_800F78F0
@L_800F78EC:
    mr r8, r9
@L_800F78F0:
    cmplwi r8, 0x0
    beqlr
    rlwinm. r0, r4, 0, 24, 31
    bnelr
    rlwinm r0, r5, 0, 24, 31
    li r3, 0x1
    mulli r0, r0, 0xf
    stw r3, 0x5c(r8)
    stw r0, 0x60(r8)
    stw r6, 0x64(r8)
    stb r7, 0x68(r8)
    blr
