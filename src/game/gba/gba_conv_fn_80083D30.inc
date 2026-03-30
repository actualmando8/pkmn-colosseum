    nofralloc
    stwu r1, -0x170(r1)
    mflr r0
    stw r0, 0x174(r1)
    stmw r25, 0x154(r1)
    mr r27, r3
    mr r28, r4
    li r31, 0x0
@L_80083D4C:
    mr r3, r27
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    li r30, 0x0
    mr r29, r3
@L_80083D60:
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    bl fn_8011F228
    bl fn_8011CA34
    mr r26, r3
    mr r3, r28
    li r4, 0x0
    li r5, 0x50
    bl memset
    cmplwi r26, 0x0
    beq @L_80083E98
    mr r3, r26
    bl fn_8011C7C0
    bl fn_800FA280
    li r4, 0x0
    mr r26, r3
    addi r3, r1, 0xac
    b @L_80083DC4
@L_80083DA8:
    cmpwi r4, 0x50
    bge @L_80083DC0
    mr r0, r4
    addi r4, r4, 0x1
    rlwinm r0, r0, 1, 0, 30
    sthx r5, r3, r0
@L_80083DC0:
    addi r26, r26, 0x2
@L_80083DC4:
    lhz r5, 0x0(r26)
    cmplwi r5, 0x0
    beq @L_80083DD8
    cmplwi r5, 0xffff
    bne @L_80083DA8
@L_80083DD8:
    rlwinm r0, r4, 1, 0, 30
    addi r4, r1, 0xac
    li r5, 0x0
    li r3, 0x0
    sthx r5, r4, r0
    li r4, 0x5
    bl fn_80135938
    mr r5, r3
    mr r3, r28
    addi r4, r1, 0xac
    bl fn_800F9AEC
    lhz r0, 0x0(r26)
    add r25, r28, r3
    cmplwi r0, 0xffff
    bne @L_80083E90
    li r0, 0xfe
    addi r6, r26, 0x3
    stb r0, 0x0(r25)
    addi r25, r25, 0x1
    li r4, 0x0
    addi r3, r1, 0x8
    b @L_80083E4C
@L_80083E30:
    cmpwi r4, 0x50
    bge @L_80083E48
    mr r0, r4
    addi r4, r4, 0x1
    rlwinm r0, r0, 1, 0, 30
    sthx r5, r3, r0
@L_80083E48:
    addi r6, r6, 0x2
@L_80083E4C:
    lhz r5, 0x0(r6)
    cmplwi r5, 0x0
    beq @L_80083E60
    cmplwi r5, 0xffff
    bne @L_80083E30
@L_80083E60:
    rlwinm r0, r4, 1, 0, 30
    addi r4, r1, 0x8
    li r5, 0x0
    li r3, 0x0
    sthx r5, r4, r0
    li r4, 0x5
    bl fn_80135938
    mr r5, r3
    mr r3, r25
    addi r4, r1, 0x8
    bl fn_800F9AEC
    add r25, r25, r3
@L_80083E90:
    li r0, 0xff
    stb r0, 0x0(r25)
@L_80083E98:
    addi r28, r28, 0x50
    addi r30, r30, 0x1
    cmpwi r30, 0x4
    blt @L_80083D60
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_80083D4C
    li r3, 0x0
    lmw r25, 0x154(r1)
    lwz r0, 0x174(r1)
    mtlr r0
    addi r1, r1, 0x170
    blr
