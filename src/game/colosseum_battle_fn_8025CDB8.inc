    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x20
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    lwz r0, -31368(r13)
    stw r3, -20940(r13)
    mulli r3, r0, 0x138
    addi r0, r3, 0x1f
    clrrwi r3, r0, 5
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    beq @L_8025CDF8
    bl fn_800E27B0
    b @L_8025CDFC
@L_8025CDF8:
    li r3, 0x0
@L_8025CDFC:
    stw r3, -20940(r13)
    mr r30, r3
    li r31, 0x0
    b @L_8025CE68
@L_8025CE0C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    addi r0, r31, 0x1
    mr r6, r3
    mr r3, r30
    li r5, 0xa
    clrlwi r4, r0, 16
    bl fn_801240C4
    addi r3, r31, 0x1004
    bl fn_800FA280
    mr r9, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0x8
    li r6, 0x1
    li r7, 0x0
    li r8, 0x0
    bl fn_80123EF0
    mr r3, r30
    bl fn_8012086C
    addi r30, r30, 0x138
    addi r31, r31, 0x1
@L_8025CE68:
    lwz r0, -31368(r13)
    cmpw r31, r0
    blt @L_8025CE0C
    lwz r3, -20944(r13)
    cmplwi r3, 0x0
    beq @L_8025CEA4
    bl fn_800E202C
    clrlwi. r0, r3, 16
    mr r30, r3
    beq @L_8025CE9C
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
@L_8025CE9C:
    li r0, 0x0
    stw r0, -20944(r13)
@L_8025CEA4:
    li r3, 0x80
    li r4, 0x20
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    beq @L_8025CEC0
    bl fn_800E27B0
    b @L_8025CEC4
@L_8025CEC0:
    li r3, 0x0
@L_8025CEC4:
    stw r3, -20944(r13)
    li r31, 0x0
@L_8025CECC:
    mr r3, r31
    bl fn_8006B09C
    addi r3, r3, 0xb44
    bl fn_8012A248
    li r30, 0x0
@L_8025CEE0:
    mr r3, r31
    bl fn_8006B09C
    clrlwi r4, r30, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    bl fn_80124A60
    addi r30, r30, 0x1
    cmpwi r30, 0x6
    blt @L_8025CEE0
    addi r31, r31, 0x1
    cmpwi r31, 0x4
    blt @L_8025CECC
    li r30, 0x0
@L_8025CF14:
    mr r3, r30
    bl fn_8006B09C
    addi r3, r3, 0x2c
    bl fn_8012A248
    li r31, 0x0
@L_8025CF28:
    mr r3, r30
    bl fn_8006B09C
    clrlwi r4, r31, 16
    addi r3, r3, 0x2c
    bl fn_8012AC08
    bl fn_80124A60
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_8025CF28
    addi r30, r30, 0x1
    cmpwi r30, 0x4
    blt @L_8025CF14
    li r0, 0x6
    mtctr r0
@L_8025CF60:
    bdnz @L_8025CF60
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr r30, r3
    li r3, 0x0
    bl fn_8006B09C
    cmplwi r30, 0x0
    addi r3, r3, 0xb44
    beq @L_8025CF94
    mr r4, r30
    li r5, 0xb18
    bl memcpy
@L_8025CF94:
    li r29, 0x0
    li r31, 0x0
@L_8025CF9C:
    cmpwi r31, 0x0
    bne @L_8025CFD8
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr r30, r3
    mr r3, r31
    bl fn_8006B09C
    cmplwi r30, 0x0
    addi r3, r3, 0xb44
    beq @L_8025D01C
    mr r4, r30
    li r5, 0xb18
    bl memcpy
    b @L_8025D01C
@L_8025CFD8:
    li r28, 0x0
@L_8025CFDC:
    lwz r0, -20940(r13)
    mr r3, r31
    add r30, r0, r29
    bl fn_8006B09C
    clrlwi r4, r28, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    cmplwi r3, 0x0
    beq @L_8025D00C
    mr r4, r30
    li r5, 0x138
    bl memcpy
@L_8025D00C:
    addi r28, r28, 0x1
    addi r29, r29, 0x138
    cmpwi r28, 0x6
    blt @L_8025CFDC
@L_8025D01C:
    addi r31, r31, 0x1
    cmpwi r31, 0x4
    blt @L_8025CF9C
    lwz r3, -20940(r13)
    cmplwi r3, 0x0
    beq @L_8025D058
    bl fn_800E202C
    clrlwi. r0, r3, 16
    mr r30, r3
    beq @L_8025D050
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
@L_8025D050:
    li r0, 0x0
    stw r0, -20940(r13)
@L_8025D058:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
