    nofralloc
    li r8, 0x0
    b @L_801AB670
@L_801AB644:
    lis r6, lbl_80465678@ha
    lis r5, lbl_80465678@ha
    rlwinm r7, r8, 3, 0, 28
    rlwinm r0, r8, 3, 0, 28
    addi r6, r6, lbl_80465678@l
    addi r5, r5, lbl_80465678@l
    add r6, r6, r7
    addi r8, r8, 0x1
    stw r3, 0x0(r6)
    add r5, r5, r0
    stw r4, 0x4(r5)
@L_801AB670:
    cmpwi r8, 0x2
    blt @L_801AB644
    blr
