    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    beq @L_8020A464
    beq @L_8020A41C
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r28
    li r4, 0x0
    li r5, 0x1f
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r28
    li r4, 0x0
    li r5, 0x20
    li r6, 0x0
    li r7, -0x1
    bl fn_80142B24
    mr r3, r28
    li r4, 0x0
    li r5, 0x21
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
@L_8020A41C:
    mr r3, r28
    rlwinm r7, r29, 0, 16, 31
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_80142B24
    mr r3, r28
    rlwinm r7, r30, 0, 16, 31
    li r4, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_80142B24
    mr r3, r28
    mr r7, r31
    li r4, 0x0
    li r5, 0x20
    li r6, 0x0
    bl fn_80142B24
@L_8020A464:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
