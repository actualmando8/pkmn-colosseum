    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r29, r5
    mr r27, r3
    li r30, 0x0
    li r4, 0x0
    li r5, 0x1
    bl fn_80235B04
    mr r31, r3
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80245E40
    cmplwi r0, 0x2
    bne @L_80245E88
@L_80245E40:
    mr r4, r27
    li r3, 0x0
    li r5, 0x136
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x136
    bl fn_80239EE8
@L_80245E88:
    clrlwi r0, r31, 24
    cmplwi r0, 0x4
    bne @L_80245EDC
    mr r3, r30
    mr r4, r27
    li r5, 0x137
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x137
    bl fn_80239EE8
@L_80245EDC:
    clrlwi r0, r31, 24
    cmplwi r0, 0x3
    bne @L_80245F30
    mr r3, r30
    mr r4, r27
    li r5, 0x138
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x138
    bl fn_80239EE8
@L_80245F30:
    mr r3, r30
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
