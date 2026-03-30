    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x58(r1)
    stmw r18, 0x20(r1)
    addi r20, r4, 0x0
    addi r22, r3, 0x0
    addi r19, r5, 0x0
    addi r4, r1, 0x18
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B3B98
    b @L_800B41AC
    @L_800B3B98:
    lwz r3, 0x18(r1)
    li r4, 0xff
    li r5, 0x2000
    lwz r27, 0x80(r3)
    mr r3, r27
    bl memset
    lis r3, 0xcc00
    lhz r18, 0x206e(r3)
    sth r20, 0x24(r27)
    bl __OSLockSram
    lwz r0, 0xc(r3)
    stw r0, 0x14(r27)
    lbz r0, 0x12(r3)
    li r3, 0x0
    stw r0, 0x18(r27)
    bl __OSUnlockSram
    bl OSGetTime
    addi r21, r4, 0x0
    addi r20, r3, 0x0
    addi r26, r21, 0x0
    addi r30, r20, 0x0
    bl __OSLockSramEx
    mulli r0, r22, 0xc
    add r25, r3, r0
    lis r3, 0x41c6
    addi r24, r25, 0x0
    addi r31, r3, 0x4e6d
    li r28, 0x0
    li r23, 0x4
    b @L_800B3F80
    @L_800B3C10:
    mullw r5, r30, r31
    mulhwu r3, r26, r31
    li r30, 0x0
    add r5, r5, r3
    mullw r3, r26, r30
    mullw r0, r26, r31
    li r29, 0x3039
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    lbz r0, 0x0(r24)
    mulhwu r5, r4, r31
    add r6, r6, r5
    addc r8, r4, r0
    extsh r7, r28
    mullw r5, r4, r30
    stbx r8, r27, r7
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    li r26, 0x7fff
    and r0, r3, r30
    and r4, r4, r26
    mullw r5, r0, r31
    mulhwu r3, r4, r31
    add r5, r5, r3
    mullw r3, r4, r30
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    mulhwu r5, r4, r31
    addi r9, r28, 0x1
    extsh r9, r9
    lbzx r0, r25, r9
    add r6, r6, r5
    mullw r5, r4, r30
    addc r7, r4, r0
    mullw r0, r4, r31
    stbx r7, r27, r9
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    and r0, r3, r30
    and r4, r4, r26
    mullw r5, r0, r31
    mulhwu r3, r4, r31
    add r5, r5, r3
    mullw r3, r4, r30
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    mulhwu r5, r4, r31
    addi r9, r28, 0x2
    extsh r9, r9
    lbzx r0, r25, r9
    add r6, r6, r5
    mullw r5, r4, r30
    addc r7, r4, r0
    mullw r0, r4, r31
    stbx r7, r27, r9
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    and r0, r3, r30
    and r4, r4, r26
    mullw r5, r0, r31
    mulhwu r3, r4, r31
    add r5, r5, r3
    mullw r3, r4, r30
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    mulhwu r5, r4, r31
    addi r9, r28, 0x3
    extsh r9, r9
    lbzx r0, r25, r9
    add r6, r6, r5
    mullw r5, r4, r30
    addc r7, r4, r0
    mullw r0, r4, r31
    stbx r7, r27, r9
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    and r0, r3, r30
    and r4, r4, r26
    mullw r5, r0, r31
    mulhwu r3, r4, r31
    add r5, r5, r3
    mullw r3, r4, r30
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    mulhwu r5, r4, r31
    addi r9, r28, 0x4
    extsh r9, r9
    lbzx r0, r25, r9
    add r6, r6, r5
    mullw r5, r4, r30
    addc r7, r4, r0
    mullw r0, r4, r31
    stbx r7, r27, r9
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    and r0, r3, r30
    and r4, r4, r26
    mullw r5, r0, r31
    mulhwu r3, r4, r31
    add r5, r5, r3
    mullw r3, r4, r30
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    mulhwu r5, r4, r31
    addi r9, r28, 0x5
    extsh r9, r9
    lbzx r0, r25, r9
    add r6, r6, r5
    mullw r5, r4, r30
    addc r7, r4, r0
    mullw r0, r4, r31
    stbx r7, r27, r9
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    and r0, r3, r30
    and r4, r4, r26
    mullw r5, r0, r31
    mulhwu r3, r4, r31
    add r5, r5, r3
    mullw r3, r4, r30
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    mulhwu r5, r4, r31
    addi r9, r28, 0x6
    extsh r9, r9
    lbzx r0, r25, r9
    add r6, r6, r5
    mullw r5, r4, r30
    addc r7, r4, r0
    mullw r0, r4, r31
    stbx r7, r27, r9
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    and r0, r3, r30
    and r4, r4, r26
    mullw r5, r0, r31
    mulhwu r3, r4, r31
    add r5, r5, r3
    mullw r3, r4, r30
    mullw r0, r4, r31
    addc r4, r0, r29
    add r0, r5, r3
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r31
    mulhwu r5, r4, r31
    addi r9, r28, 0x7
    extsh r9, r9
    lbzx r0, r25, r9
    add r6, r6, r5
    mullw r5, r4, r30
    addc r7, r4, r0
    mullw r0, r4, r31
    stbx r7, r27, r9
    addc r4, r0, r29
    add r0, r6, r5
    adde r3, r0, r30
    li r5, 0x10
    bl fn_800C4C98
    and r26, r4, r26
    and r30, r3, r30
    addi r24, r24, 0x8
    addi r28, r28, 0x8
    @L_800B3F80:
    extsh r0, r28
    cmpw r0, r23
    blt @L_800B3C10
    b @L_800B41A4
    @L_800B3F90:
    lis r3, 0x41c6
    addi r25, r3, 0x4e6d
    b @L_800B4018
    @L_800B3F9C:
    mullw r5, r30, r25
    mulhwu r3, r26, r25
    li r24, 0x0
    add r5, r5, r3
    mullw r3, r26, r24
    mullw r0, r26, r25
    li r23, 0x3039
    addc r4, r0, r23
    add r0, r5, r3
    adde r3, r0, r24
    li r5, 0x10
    bl fn_800C4C98
    mullw r6, r3, r25
    lbz r0, 0x0(r29)
    mulhwu r5, r4, r25
    add r6, r6, r5
    addc r8, r4, r0
    extsh r7, r28
    mullw r5, r4, r24
    stbx r8, r27, r7
    mullw r0, r4, r25
    addc r4, r0, r23
    add r0, r6, r5
    adde r3, r0, r24
    li r5, 0x10
    bl fn_800C4C98
    li r0, 0x7fff
    and r26, r4, r0
    and r30, r3, r24
    addi r29, r29, 0x1
    addi r28, r28, 0x1
    @L_800B4018:
    extsh r0, r28
    cmpwi r0, 0xc
    blt @L_800B3F9C
    li r3, 0x0
    bl __OSUnlockSramEx
    stw r18, 0x1c(r27)
    li r18, 0x0
    addi r3, r27, 0x0
    stw r21, 0x10(r27)
    addi r5, r27, 0x1fc
    addi r6, r27, 0x1fe
    stw r20, 0xc(r27)
    li r4, 0x1fc
    sth r18, 0x20(r27)
    lwz r7, 0x18(r1)
    lhz r0, 0x8(r7)
    sth r0, 0x22(r27)
    bl fn_800B2070
    b @L_800B40AC
    @L_800B4064:
    lwz r4, 0x18(r1)
    extsh r3, r18
    addi r0, r3, 0x1
    lwz r3, 0x80(r4)
    slwi r0, r0, 13
    li r4, 0xff
    add r20, r3, r0
    addi r3, r20, 0x0
    li r5, 0x2000
    bl memset
    addi r4, r20, 0x1fc0
    sth r18, 0x1ffa(r20)
    addi r3, r20, 0x0
    addi r5, r4, 0x3c
    addi r6, r4, 0x3e
    li r4, 0x1ffc
    bl fn_800B2070
    addi r18, r18, 0x1
    @L_800B40AC:
    extsh r0, r18
    cmpwi r0, 0x2
    blt @L_800B4064
    li r18, 0x0
    b @L_800B411C
    @L_800B40C0:
    lwz r4, 0x18(r1)
    extsh r3, r18
    addi r0, r3, 0x3
    lwz r3, 0x80(r4)
    slwi r0, r0, 13
    li r4, 0x0
    add r20, r3, r0
    addi r3, r20, 0x0
    li r5, 0x2000
    bl memset
    sth r18, 0x4(r20)
    li r0, 0x4
    addi r5, r20, 0x0
    lwz r4, 0x18(r1)
    addi r3, r20, 0x4
    addi r6, r20, 0x2
    lhz r7, 0x10(r4)
    li r4, 0x1ffc
    subi r7, r7, 0x5
    sth r7, 0x6(r20)
    sth r0, 0x8(r20)
    bl fn_800B2070
    addi r18, r18, 0x1
    @L_800B411C:
    extsh r0, r18
    cmpwi r0, 0x2
    blt @L_800B40C0
    cmplwi r19, 0x0
    beq @L_800B4138
    mr r0, r19
    b @L_800B4140
    @L_800B4138:
    lis r3, fn_800AF280@ha
    addi r0, r3, fn_800AF280@l
    @L_800B4140:
    lwz r5, 0x18(r1)
    lis r3, 0x1
    subi r4, r3, 0x6000
    stw r0, 0xd0(r5)
    lwz r3, 0x18(r1)
    lwz r3, 0x80(r3)
    bl fn_8009B300
    lwz r4, 0x18(r1)
    li r0, 0x0
    lis r3, fn_800B3A24@ha
    stw r0, 0x28(r4)
    addi r5, r3, fn_800B3A24@l
    addi r3, r22, 0x0
    lwz r6, 0x18(r1)
    lwz r4, 0xc(r6)
    lwz r0, 0x28(r6)
    mullw r4, r4, r0
    bl fn_800AFFE0
    mr. r18, r3
    bge @L_800B419C
    lwz r3, 0x18(r1)
    mr r4, r18
    bl fn_800B02F4
    @L_800B419C:
    mr r3, r18
    b @L_800B41AC
    @L_800B41A4:
    add r29, r25, r0
    b @L_800B3F90
    @L_800B41AC:
    lmw r18, 0x20(r1)
    lwz r0, 0x5c(r1)
    addi r1, r1, 0x58
    mtlr r0
    blr
