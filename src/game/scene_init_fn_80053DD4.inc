    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    li r30, 0x0
    bl fn_80057270
    cmplwi r3, 0x0
    beq @L_80053E58
    bl fn_8011F4A8
    clrlwi r31, r3, 24
    cmpwi r31, 0x64
    bge @L_80053E18
    li r30, 0x2
    b @L_80053E1C
    @L_80053E18:
    li r30, 0x3
    @L_80053E1C:
    li r3, 0x1b82
    bl fn_800FA444
    mulli r5, r30, 0xf
    lha r0, 0x54(r29)
    srwi r3, r3, 16
    li r4, 0x0
    subf r0, r5, r0
    li r5, -0x1
    subf r3, r3, r0
    li r6, 0x1b82
    bl fn_800FB680
    mr r4, r31
    li r3, 0x34
    bl fn_80132A38
    li r30, 0xde
    @L_80053E58:
    stw r30, 0x4c(r29)
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
