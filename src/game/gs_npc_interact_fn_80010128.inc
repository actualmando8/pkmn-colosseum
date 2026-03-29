    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r4
    lwz r30, 0x40(r3)
    mr r3, r30
    bl fn_80205B8C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    mr r4, r27
    addi r6, r1, 0x8
    li r5, 0x1
    bl fn_801FFEC8
    mr r31, r3
    mr r3, r29
    mr r6, r27
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    clrlwi r0, r31, 24
    clrlwi r29, r3, 16
    cmplwi r0, 0x0
    beq @L_800101E4
    mr r4, r30
    li r3, 0x11
    bl fn_80132A38
    mr r4, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    mr r28, r3
    bl fn_800FA280
    mr r4, r3
    li r3, 0x28
    bl fn_80132A38
    mr r3, r30
    bl fn_802040E8
    mr r0, r3
    li r3, 0x0
    clrlwi r7, r0, 16
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    @L_800101E4:
    clrlwi r0, r31, 24
    cmplwi r0, 0x6
    bne @L_800101F8
    li r28, 0x7661
    b @L_80010268
    @L_800101F8:
    cmplwi r0, 0x5
    bne @L_8001022C
    lhz r4, 0x8(r1)
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x28
    bl fn_80132A38
    li r28, 0x76bb
    b @L_80010268
    @L_8001022C:
    cmplwi r0, 0x4
    bne @L_8001023C
    li r28, 0x7600
    b @L_80010268
    @L_8001023C:
    cmplwi r0, 0x3
    bne @L_8001024C
    li r28, 0x75ff
    b @L_80010268
    @L_8001024C:
    cmplwi r0, 0x2
    bne @L_8001025C
    li r28, 0x75fe
    b @L_80010268
    @L_8001025C:
    cmplwi r0, 0x1
    bne @L_80010268
    li r28, 0x75fd
    @L_80010268:
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    beq @L_8001027C
    mr r3, r28
    b @L_80010280
    @L_8001027C:
    li r3, 0x0
    @L_80010280:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
