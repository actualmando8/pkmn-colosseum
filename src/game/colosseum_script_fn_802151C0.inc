    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r31, r0
    li r4, 0x0
    bl fn_801F025C
    bl fn_80203D3C
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x5f
    li r6, 0x0
    bl fn_8012640C
    lis r4, lbl_80279F88@ha
    rlwinm r5, r3, 0, 16, 31
    addi r3, r4, lbl_80279F88@l
    li r8, 0x0
    b @L_8021523C
@L_80215230:
    cmplw r0, r5
    bgt @L_8021524C
    addi r8, r8, 0x2
@L_8021523C:
    clrlslwi r0, r8, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0xffff
    bne @L_80215230
@L_8021524C:
    cmplwi r0, 0xffff
    mr r3, r31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    beq @L_8021527C
    lis r7, lbl_80279F88@ha
    clrlslwi r0, r8, 16, 1
    addi r7, r7, lbl_80279F88@l
    add r7, r7, r0
    lhz r0, 0x2(r7)
    b @L_80215280
@L_8021527C:
    li r0, 0x78
@L_80215280:
    rlwinm r7, r0, 0, 16, 31
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
