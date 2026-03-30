    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0xee
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r4
    mr r29, r3
    li r4, 0x0
    bl fn_8012640C
    rlwinm. r0, r30, 0, 16, 31
    mr r31, r3
    bne @L_80202864
    cmplwi r31, 0x0
    beq @L_80202894
    li r4, 0x1
    bl fn_801DA36C
    mr r3, r31
    li r4, 0x2
    bl fn_801DA36C
    b @L_80202894
@L_80202864:
    cmplwi r31, 0x0
    beq @L_80202894
    cmplwi r0, 0x8
    bne @L_8020287C
    li r4, 0x1
    bl fn_801DA36C
@L_8020287C:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x7
    bne @L_80202894
    mr r3, r31
    li r4, 0x2
    bl fn_801DA36C
@L_80202894:
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_802028D0
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_802028D0
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80202964
@L_802028D0:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80202910
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80202940
@L_80202910:
    cmplwi r31, 0x0
    bne @L_80202920
    li r3, 0x0
    b @L_80202934
@L_80202920:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80202934:
    mr r4, r30
    bl fn_80121B4C
    b @L_80202984
@L_80202940:
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80202984
    mr r3, r31
    mr r4, r30
    bl fn_8011B788
    b @L_80202984
@L_80202964:
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    bne @L_80202984
    mr r3, r29
    mr r4, r30
    bl fn_8011B788
@L_80202984:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
