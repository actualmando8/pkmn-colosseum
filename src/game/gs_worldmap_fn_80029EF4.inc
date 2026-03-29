    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    mr r30, r5
    mr r31, r7
    clrlwi r0, r6, 24
    cmpwi r0, 0x3
    beq @L_80029F64
    bge @L_80029F70
    cmpwi r0, 0x2
    bge @L_80029F34
    b @L_80029F70
    @L_80029F34:
    mr r4, r3
    li r3, 0x0
    bl fn_80129384
    mr r4, r29
    clrlwi r5, r30, 16
    li r3, 0x0
    bl fn_8013467C
    cmplwi r31, 0x0
    beq @L_80029F90
    li r0, 0x1
    stb r0, 0x760(r31)
    b @L_80029F90
    @L_80029F64:
    mr r6, r31
    bl fn_80029AC8
    b @L_80029F90
    @L_80029F70:
    mr r4, r3
    li r3, 0x0
    bl fn_80129384
    mr r4, r29
    clrlwi r5, r30, 16
    li r3, 0x0
    li r6, -0x1
    bl fn_80129A78
    @L_80029F90:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
