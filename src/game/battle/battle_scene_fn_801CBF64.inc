    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r9, 0x0
    li r7, 0x4
    stw r0, 0x24(r1)
    li r0, 0x2
    addi r8, r1, 0x8
    stw r31, 0x1c(r1)
    mr r31, r4
    stw r30, 0x18(r1)
    mr r30, r3
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mtctr r0
@L_801CBF9C:
    xor r6, r7, r9
    rlwinm r4, r9, 0, 30, 31
    cntlzw r6, r6
    addi r9, r9, 0x1
    slw r5, r7, r6
    subfic r3, r4, 0x3
    rlwinm r5, r5, 3, 29, 29
    xor r6, r7, r9
    addi r5, r5, 0x14
    rlwinm r4, r9, 0, 30, 31
    lwzx r5, r31, r5
    rlwinm r0, r3, 3, 0, 28
    cntlzw r6, r6
    addi r9, r9, 0x1
    srw r0, r5, r0
    subfic r3, r4, 0x3
    slw r5, r7, r6
    stb r0, 0x0(r8)
    rlwinm r5, r5, 3, 29, 29
    xor r6, r7, r9
    addi r5, r5, 0x14
    rlwinm r4, r9, 0, 30, 31
    rlwinm r0, r3, 3, 0, 28
    lwzx r5, r31, r5
    cntlzw r6, r6
    addi r9, r9, 0x1
    srw r0, r5, r0
    subfic r3, r4, 0x3
    slw r5, r7, r6
    stb r0, 0x1(r8)
    rlwinm r5, r5, 3, 29, 29
    xor r6, r7, r9
    addi r5, r5, 0x14
    rlwinm r0, r3, 3, 0, 28
    lwzx r5, r31, r5
    cntlzw r6, r6
    rlwinm r4, r9, 0, 30, 31
    addi r9, r9, 0x1
    srw r0, r5, r0
    slw r5, r7, r6
    rlwinm r5, r5, 3, 29, 29
    stb r0, 0x2(r8)
    addi r5, r5, 0x14
    subfic r3, r4, 0x3
    lwzx r5, r31, r5
    rlwinm r0, r3, 3, 0, 28
    srw r0, r5, r0
    stb r0, 0x3(r8)
    addi r8, r8, 0x4
    bdnz @L_801CBF9C
    lwz r3, 0x14(r31)
    addi r0, r3, 0x8
    rlwinm r4, r3, 29, 26, 31
    cmplwi r0, 0x8
    stw r0, 0x14(r31)
    bge @L_801CC088
    lwz r3, 0x18(r31)
    addi r0, r3, 0x1
    stw r0, 0x18(r31)
@L_801CC088:
    addi r0, r4, 0x1
    cmplwi r0, 0x3f
    ble @L_801CC0EC
    addi r3, r4, 0x1c
    subfic r29, r4, 0x40
    mr r5, r29
    la r4, lbl_8047E160(r0)
    add r3, r31, r3
    bl memcpy
    mr r3, r31
    addi r4, r31, 0x1c
    bl fn_801CC380
    la r28, lbl_8047E160(r0)
    add r28, r28, r29
    b @L_801CC0D8
@L_801CC0C4:
    mr r3, r31
    mr r4, r28
    bl fn_801CC380
    addi r28, r28, 0x40
    addi r29, r29, 0x40
@L_801CC0D8:
    addi r0, r29, 0x3f
    cmplwi r0, 0x1
    blt @L_801CC0C4
    li r4, 0x0
    b @L_801CC0F0
@L_801CC0EC:
    li r29, 0x0
@L_801CC0F0:
    addi r3, r4, 0x1c
    la r4, lbl_8047E160(r0)
    add r3, r31, r3
    subfic r5, r29, 0x1
    add r4, r4, r29
    bl memcpy
    b @L_801CC1B0
@L_801CC10C:
    lwz r3, 0x14(r31)
    rlwinm r4, r4, 29, 26, 31
    addi r0, r3, 0x8
    cmplwi r0, 0x8
    stw r0, 0x14(r31)
    bge @L_801CC130
    lwz r3, 0x18(r31)
    addi r0, r3, 0x1
    stw r0, 0x18(r31)
@L_801CC130:
    addi r0, r4, 0x1
    cmplwi r0, 0x3f
    ble @L_801CC194
    addi r3, r4, 0x1c
    subfic r29, r4, 0x40
    mr r5, r29
    la r4, lbl_8047E164(r0)
    add r3, r31, r3
    bl memcpy
    mr r3, r31
    addi r4, r31, 0x1c
    bl fn_801CC380
    la r28, lbl_8047E164(r0)
    add r28, r28, r29
    b @L_801CC180
@L_801CC16C:
    mr r3, r31
    mr r4, r28
    bl fn_801CC380
    addi r28, r28, 0x40
    addi r29, r29, 0x40
@L_801CC180:
    addi r0, r29, 0x3f
    cmplwi r0, 0x1
    blt @L_801CC16C
    li r4, 0x0
    b @L_801CC198
@L_801CC194:
    li r29, 0x0
@L_801CC198:
    addi r3, r4, 0x1c
    la r4, lbl_8047E164(r0)
    add r3, r31, r3
    subfic r5, r29, 0x1
    add r4, r4, r29
    bl memcpy
@L_801CC1B0:
    lwz r4, 0x14(r31)
    rlwinm r0, r4, 0, 23, 28
    cmplwi r0, 0x1c0
    bne @L_801CC10C
    addi r0, r4, 0x40
    rlwinm r4, r4, 29, 26, 31
    cmplwi r0, 0x40
    stw r0, 0x14(r31)
    bge @L_801CC1E0
    lwz r3, 0x18(r31)
    addi r0, r3, 0x1
    stw r0, 0x18(r31)
@L_801CC1E0:
    addi r0, r4, 0x8
    cmplwi r0, 0x3f
    ble @L_801CC244
    addi r3, r4, 0x1c
    subfic r29, r4, 0x40
    mr r5, r29
    addi r4, r1, 0x8
    add r3, r31, r3
    bl memcpy
    mr r3, r31
    addi r4, r31, 0x1c
    bl fn_801CC380
    addi r28, r1, 0x8
    add r28, r28, r29
    b @L_801CC230
@L_801CC21C:
    mr r3, r31
    mr r4, r28
    bl fn_801CC380
    addi r28, r28, 0x40
    addi r29, r29, 0x40
@L_801CC230:
    addi r0, r29, 0x3f
    cmplwi r0, 0x8
    blt @L_801CC21C
    li r4, 0x0
    b @L_801CC248
@L_801CC244:
    li r29, 0x0
@L_801CC248:
    addi r3, r4, 0x1c
    addi r4, r1, 0x8
    add r3, r31, r3
    subfic r5, r29, 0x8
    add r4, r4, r29
    bl memcpy
    li r0, 0x4
    li r4, 0x0
    mtctr r0
@L_801CC26C:
    rlwinm r3, r4, 0, 0, 29
    rlwinm r0, r4, 0, 30, 31
    subfic r0, r0, 0x3
    lwzx r3, r31, r3
    rlwinm r0, r0, 3, 0, 28
    addi r5, r4, 0x1
    srw r0, r3, r0
    add r3, r30, r4
    stb r0, 0x0(r3)
    rlwinm r6, r5, 0, 0, 29
    rlwinm r5, r5, 0, 30, 31
    addi r0, r4, 0x2
    subfic r5, r5, 0x3
    lwzx r7, r31, r6
    rlwinm r5, r5, 3, 0, 28
    addi r6, r4, 0x3
    srw r7, r7, r5
    stb r7, 0x1(r3)
    rlwinm r7, r0, 0, 0, 29
    rlwinm r0, r0, 0, 30, 31
    addi r5, r4, 0x4
    subfic r0, r0, 0x3
    lwzx r8, r31, r7
    rlwinm r7, r0, 3, 0, 28
    addi r4, r4, 0x5
    srw r8, r8, r7
    rlwinm r0, r6, 0, 30, 31
    stb r8, 0x2(r3)
    rlwinm r7, r6, 0, 0, 29
    subfic r6, r0, 0x3
    rlwinm r0, r5, 0, 30, 31
    lwzx r7, r31, r7
    rlwinm r6, r6, 3, 0, 28
    subfic r0, r0, 0x3
    rlwinm r5, r5, 0, 0, 29
    srw r6, r7, r6
    stb r6, 0x3(r3)
    rlwinm r0, r0, 3, 0, 28
    lwzx r5, r31, r5
    srw r0, r5, r0
    stb r0, 0x4(r3)
    bdnz @L_801CC26C
    addi r3, r31, 0x1c
    li r4, 0x0
    li r5, 0x40
    bl memset
    mr r3, r31
    li r4, 0x0
    li r5, 0x14
    bl memset
    addi r3, r31, 0x14
    li r4, 0x0
    li r5, 0x8
    bl memset
    addi r3, r1, 0x8
    li r4, 0x0
    li r5, 0x8
    bl memset
    mr r3, r31
    addi r4, r31, 0x1c
    bl fn_801CC380
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
