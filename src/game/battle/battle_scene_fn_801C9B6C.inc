    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_801C9BB4
    cmplwi r0, 0x5
    bgt @L_801C9BB4
    bl fn_801C8E14
    b @L_801C9C7C
@L_801C9BB4:
    rlwinm r0, r30, 0, 16, 31
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_801C9C10
    addi r3, r1, 0xc
    addi r4, r1, 0x8
    li r5, 0x1
    bl fn_8012EFB8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9BFC
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C9C08
    bl fn_801C9910
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801C9C08
@L_801C9BFC:
    li r0, 0x0
    stb r0, -21596(r13)
    b @L_801C9C10
@L_801C9C08:
    li r0, 0x1
    stb r0, -21596(r13)
@L_801C9C10:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801C9C40
    addi r3, r1, 0xc
    addi r4, r1, 0x8
    li r5, 0x1
    bl fn_8012EFB8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9C40
    li r0, 0x1
    stb r0, -21596(r13)
@L_801C9C40:
    lbz r0, -21596(r13)
    cmplwi r0, 0x0
    beq @L_801C9C64
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    bl fn_801C8E14
    b @L_801C9C7C
@L_801C9C64:
    mr r3, r28
    mr r4, r29
    mr r5, r30
    li r6, 0x0
    li r7, 0x64
    bl fn_80116470
@L_801C9C7C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
