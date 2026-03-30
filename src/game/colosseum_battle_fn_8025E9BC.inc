    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    clrlwi r5, r3, 16
    li r4, 0x3
    stw r0, 0x34(r1)
    li r0, 0x0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r30, r3
    li r3, 0x0
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    sth r0, 0x8(r1)
    bl fn_8012A5B0
    mr r31, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025ED2C
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    mr r3, r31
    bl fn_8011EE40
    li r4, 0x1
    bl fn_801EECD8
    mr r3, r31
    li r4, 0x3e
    bl fn_80121ADC
    clrlwi. r0, r3, 24
    beq @L_8025EA4C
    mr r3, r31
    li r4, 0x3e
    bl fn_80121B4C
@L_8025EA4C:
    lfs f1, -20504(r2)
    mr r3, r31
    bl fn_8011FBCC
    li r3, 0x3f7
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    mr r3, r31
    li r4, 0x0
    bl fn_8011F228
    clrlwi. r0, r3, 16
    mr r29, r3
    beq @L_8025EAB4
    mr r3, r31
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x32
    mr r4, r0
    bl fn_80132A38
    clrlwi r4, r29, 16
    li r3, 0x39
    bl fn_80132A38
    li r3, 0x3b10
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
@L_8025EAB4:
    mr r3, r31
    bl fn_8011EE10
    mr r0, r3
    li r3, 0x2f
    mr r28, r0
    mr r4, r28
    bl fn_80132A38
    li r3, 0x3b0b
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801065B8
    mr r3, r31
    bl fn_8011EE58
    mr r29, r3
    mr r3, r31
    bl fn_8011EDF8
    addi r0, r3, 0x46
    mr r3, r31
    add r0, r29, r0
    clrlwi r4, r0, 16
    bl fn_8011D904
    mr r3, r31
    li r4, 0x0
    bl fn_8011DE68
    mr r3, r31
    mr r4, r28
    bl fn_8025E640
    cmpwi r3, 0x1
    bne @L_8025EC64
    mr r3, r31
    addi r6, r1, 0x8
    addi r7, r1, 0x18
    li r4, 0x0
    li r5, 0x0
    bl fn_80128A64
    clrlwi. r0, r3, 16
    mr r28, r3
    beq @L_8025EC64
    cmplwi r0, 0xffff
    beq @L_8025EC64
    lfs f1, -20512(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    bl fn_801DAC90
    lhz r5, 0x8(r1)
    mr r3, r31
    mr r4, r28
    addi r6, r1, 0x18
    li r7, 0x0
    li r8, 0x1
    li r9, 0x1
    li r10, 0x0
    bl fn_8012805C
    cmpwi r3, 0x0
    bne @L_8025EC04
    li r3, 0x1
    bl fn_801DADC0
    mr r4, r31
    addi r3, r1, 0x14
    addi r5, r13, -31344
    li r6, 0x1
    bl fn_8025E0B0
    cmpwi r3, 0x1
    bne @L_8025EBEC
    lfs f1, -20512(r2)
    li r3, 0x2
    bl fn_801C41C8
    addi r3, r1, 0x14
    addi r4, r13, -31344
    li r5, 0x1
    li r6, 0x1
    li r7, 0x1
    li r8, 0x0
    bl fn_8025DE54
@L_8025EBEC:
    mr r3, r31
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    b @L_8025EC64
@L_8025EC04:
    mr r3, r31
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x1
    bl fn_801DADC0
    mr r4, r31
    addi r3, r1, 0x10
    addi r5, r13, -31344
    li r6, 0x1
    bl fn_8025E0B0
    cmpwi r3, 0x1
    bne @L_8025EC64
    lfs f1, -20512(r2)
    li r3, 0x2
    bl fn_801C41C8
    addi r3, r1, 0x10
    addi r4, r13, -31344
    li r5, 0x1
    li r6, 0x1
    li r7, 0x1
    li r8, 0x0
    bl fn_8025DE54
@L_8025EC64:
    li r3, 0x3ca
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    mr r3, r31
    bl fn_80265E7C
    li r3, 0x3b0c
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x3b0d
    li r4, 0x1
    li r5, 0x1
    bl fn_801067E8
    bl fn_8001E184
    extsb. r0, r3
    beq @L_8025ECB4
    li r3, 0x1
    bl fn_801065B8
    b @L_8025ED2C
@L_8025ECB4:
    li r3, 0x1
    bl fn_801065B8
    lfs f1, -20512(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    bl fn_801DAC90
    mr r4, r30
    li r3, 0x2
    bl fn_80029660
    li r3, 0x1
    bl fn_801DADC0
    mr r4, r31
    addi r3, r1, 0xc
    addi r5, r13, -31344
    li r6, 0x1
    bl fn_8025E0B0
    cmpwi r3, 0x1
    bne @L_8025ED2C
    lfs f1, -20512(r2)
    li r3, 0x2
    bl fn_801C41C8
    addi r3, r1, 0xc
    addi r4, r13, -31344
    li r5, 0x1
    li r6, 0x1
    li r7, 0x1
    li r8, 0x0
    bl fn_8025DE54
@L_8025ED2C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
