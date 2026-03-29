    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    lwz r0, 0x0(r29)
    cmpwi r0, 0x0
    beq @L_80058F6C
    b @L_80059020
    @L_80058F6C:
    cmpwi r28, 0x1
    bne @L_80058FC4
    lwz r3, 0x4(r29)
    subi r31, r3, 0x1
    b @L_80058FA8
    @L_80058F80:
    clrlwi r5, r31, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80058FB0
    subi r31, r31, 0x1
    @L_80058FA8:
    cmpwi r31, 0x0
    bge @L_80058F80
    @L_80058FB0:
    cmpwi r31, 0x0
    blt @L_80058FC4
    stw r31, 0x4(r29)
    mr r3, r30
    b @L_80059020
    @L_80058FC4:
    cmpwi r28, 0x2
    bne @L_8005901C
    lwz r3, 0x4(r29)
    addi r31, r3, 0x1
    b @L_80059000
    @L_80058FD8:
    clrlwi r5, r31, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80059008
    addi r31, r31, 0x1
    @L_80059000:
    cmpwi r31, 0x6
    blt @L_80058FD8
    @L_80059008:
    cmpwi r31, 0x6
    bge @L_8005901C
    stw r31, 0x4(r29)
    mr r3, r30
    b @L_80059020
    @L_8005901C:
    mr r3, r27
    @L_80059020:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
