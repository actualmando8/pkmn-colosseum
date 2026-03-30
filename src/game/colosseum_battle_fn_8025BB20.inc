    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r26, 0x68(r1)
    mr r27, r4
    mr r28, r5
    mr r26, r3
    mr r29, r6
    mr r5, r27
    mr r4, r28
    bl fn_802395C8
    mr r30, r3
    mr r4, r26
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1A6C
    mr r31, r3
    mr r3, r28
    bl fn_8022967C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025BB94
    clrlwi r0, r31, 16
    cmplwi r0, 0x1
    bgt @L_8025BB94
    li r3, 0x0
    b @L_8025BC14
@L_8025BB94:
    mr r3, r26
    bl fn_8025CBE8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025BBB0
    li r3, 0x0
    b @L_8025BC14
@L_8025BBB0:
    mr r3, r26
    mr r4, r28
    bl fn_80239500
    mr r6, r3
    mr r3, r26
    mr r4, r29
    mr r5, r30
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_8025BBE4
    li r3, 0x0
    b @L_8025BC14
@L_8025BBE4:
    mr r3, r26
    mr r4, r27
    mr r5, r28
    mr r6, r29
    li r7, 0x0
    bl fn_8025C264
    cmpwi r3, 0x0
    bne @L_8025BC0C
    li r3, 0x0
    b @L_8025BC14
@L_8025BC0C:
    cmpwi r3, -0x1
    li r3, 0x1
@L_8025BC14:
    lmw r26, 0x68(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
