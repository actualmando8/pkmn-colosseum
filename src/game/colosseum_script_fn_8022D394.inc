    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, lbl_8037994A@ha
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r31, r3
    addi r29, r4, lbl_8037994A@l
@L_8022D3B0:
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022D3C8
    li r3, 0x1
    b @L_8022D6A8
@L_8022D3C8:
    mr r3, r31
    bl fn_80207BF4
    mr r30, r3
    mr r3, r31
    bl fn_80205B8C
    rlwinm r0, r30, 0, 16, 31
    li r28, 0x0
    cmpwi r0, 0x11
    mr r30, r3
    beq @L_8022D448
    bge @L_8022D418
    cmpwi r0, 0xc
    beq @L_8022D5BC
    bge @L_8022D40C
    cmpwi r0, 0x7
    beq @L_8022D4D0
    b @L_8022D5F0
@L_8022D40C:
    cmpwi r0, 0xf
    beq @L_8022D508
    b @L_8022D5F0
@L_8022D418:
    cmpwi r0, 0x29
    beq @L_8022D54C
    bge @L_8022D43C
    cmpwi r0, 0x14
    beq @L_8022D498
    blt @L_8022D5F0
    cmpwi r0, 0x28
    bge @L_8022D584
    b @L_8022D5F0
@L_8022D43C:
    cmpwi r0, 0x48
    beq @L_8022D508
    b @L_8022D5F0
@L_8022D448:
    mr r3, r31
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022D478
    mr r3, r31
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D5F0
@L_8022D478:
    li r3, 0x3
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r28, 0x1
    b @L_8022D5F0
@L_8022D498:
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D5F0
    li r3, 0x9
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r28, 0x2
    b @L_8022D5F0
@L_8022D4D0:
    mr r3, r31
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D5F0
    li r3, 0x5
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r28, 0x1
    b @L_8022D5F0
@L_8022D508:
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D5F0
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    li r3, 0x8
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r28, 0x1
    b @L_8022D5F0
@L_8022D54C:
    mr r3, r31
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D5F0
    li r3, 0x6
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r28, 0x1
    b @L_8022D5F0
@L_8022D584:
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D5F0
    li r3, 0x7
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r28, 0x1
    b @L_8022D5F0
@L_8022D5BC:
    mr r3, r31
    li r4, 0xa
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D5F0
    li r3, 0xa
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r28, 0x3
@L_8022D5F0:
    rlwinm. r0, r28, 0, 24, 31
    beq @L_8022D6A4
    cmpwi r0, 0x2
    beq @L_8022D634
    bge @L_8022D610
    cmpwi r0, 0x1
    bge @L_8022D61C
    b @L_8022D650
@L_8022D610:
    cmpwi r0, 0x4
    bge @L_8022D650
    b @L_8022D644
@L_8022D61C:
    mr r3, r30
    bl fn_801252E0
    mr r3, r31
    li r4, 0x0
    bl fn_80202998
    b @L_8022D650
@L_8022D634:
    mr r3, r31
    li r4, 0x9
    bl fn_80202810
    b @L_8022D650
@L_8022D644:
    mr r3, r31
    li r4, 0xa
    bl fn_80202810
@L_8022D650:
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D678
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022D678:
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B62C(r13)
    mr r4, r29
    li r5, 0x0
    bl fn_80211B94
    b @L_8022D3B0
@L_8022D6A4:
    li r3, 0x1
@L_8022D6A8:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
