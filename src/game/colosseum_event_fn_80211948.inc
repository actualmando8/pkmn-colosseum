    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    mr r11, r4
    mr r10, r5
    stw r0, 0x44(r1)
    mr r9, r6
    mr r0, r7
    mr r4, r3
    stw r31, 0x3c(r1)
    mr r31, r8
    mr r5, r11
    mr r6, r10
    mr r7, r9
    mr r8, r0
    addi r3, r1, 0x8
    bl fn_801F11CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80211998
    b @L_802119A8
@L_80211998:
    mr r4, r31
    addi r3, r1, 0x8
    bl fn_8020D888
    li r3, 0x1
@L_802119A8:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802119B8
    b @L_802119C0
@L_802119B8:
    addi r3, r1, 0x8
    bl fn_801F0F04
@L_802119C0:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
