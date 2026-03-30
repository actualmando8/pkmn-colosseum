    nofralloc
    cmpwi r3, 0x2
    bgelr
    cmpwi r3, 0x0
    blt @L_801AB610
    cmpwi r3, 0x2
    bltlr
@L_801AB610:
    lis r7, lbl_80465678@ha
    lis r6, lbl_80465678@ha
    rlwinm r8, r3, 3, 0, 28
    rlwinm r0, r3, 3, 0, 28
    addi r7, r7, lbl_80465678@l
    addi r3, r6, lbl_80465678@l
    add r6, r7, r8
    stw r4, 0x0(r6)
    add r3, r3, r0
    stw r5, 0x4(r3)
    blr
